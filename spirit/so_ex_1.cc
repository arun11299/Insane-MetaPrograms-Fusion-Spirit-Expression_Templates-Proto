//#define BOOST_SPIRIT_X3_DEBUG
#include <iostream>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <string>
#include <vector>

//--------------------------------------------------------------------------------------------------
namespace client { namespace ast
{
    namespace fusion = boost::fusion;
    namespace x3 = boost::spirit::x3;

    struct number : x3::variant<int, double> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct add_ast;
    struct mult_ast;
    struct block_ast;
    struct function;

    struct expr_ast : x3::variant<
            number,
            x3::forward_ast<function>,
            x3::forward_ast<add_ast>,
            x3::forward_ast<mult_ast>,
            x3::forward_ast<block_ast>
        > {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct add_ast {
        expr_ast lhs;
        bool     add;
        expr_ast rhs;
    };

    struct mult_ast {
        expr_ast lhs;
        bool     mult;
        expr_ast rhs;
    };

    struct block_ast {
        expr_ast body;
    };

    struct function {
        std::string           name;
        std::vector<expr_ast> params;
    };
}}

//--------------------------------------------------------------------------------------------------
BOOST_FUSION_ADAPT_STRUCT(client::ast::add_ast,
    (client::ast::expr_ast, lhs),
    (bool, add),
    (client::ast::expr_ast, rhs)
)
BOOST_FUSION_ADAPT_STRUCT(client::ast::mult_ast,
    (client::ast::expr_ast, lhs),
    (bool, mult),
    (client::ast::expr_ast, rhs)
)
BOOST_FUSION_ADAPT_STRUCT(client::ast::block_ast,
    (client::ast::expr_ast, body)
)
BOOST_FUSION_ADAPT_STRUCT(client::ast::function,
    (std::string, name),
    (std::vector<client::ast::expr_ast>, params)
)

//--------------------------------------------------------------------------------------------------
namespace client { namespace parser
{
    namespace x3 = boost::spirit::x3;

    const x3::rule<class expr,       ast::expr_ast> expr       = "expr";
    const x3::rule<class add_expr,   ast::expr_ast> add_expr   = "add_expr";
    const x3::rule<class mult_expr,  ast::expr_ast> mult_expr  = "mult_expr";
    const x3::rule<class block_expr, ast::expr_ast> block_expr = "block_expr";

    auto const number   = x3::rule<class number, ast::number> {"number"}
                        = (x3::int_ >> !x3::lit('.')) | x3::double_;

    auto const fct_name = x3::rule<class fct_name, std::string> {"fct_name"}
                        = x3::lexeme[ *x3::alpha >> *(x3::alnum | x3::char_('_')) ];

    auto const function = x3::rule<class function, ast::function> {"function"}
                        = fct_name >> x3::lit("(") >> -expr % ',' >> ")";

    auto const simple_expr = x3::rule<class simple_expr, ast::expr_ast> {"simple_expr"}
                           = function | number;

    auto const block_term = x3::rule<class block_term, ast::block_ast> {"block_term"}
                          = "(" >> expr >> ")";

    auto const mult_term = x3::rule<class mult_term, ast::mult_ast> {"mult_term"}
                         = block_expr
                           >> ((x3::lit("*") >> x3::attr(true)) | (x3::lit("/") >> x3::attr(false)))
                           >> mult_expr;

    auto const add_term = x3::rule<class add_term, ast::add_ast> {"add_term"}
                        = mult_expr
                          >> ((x3::lit("+") >> x3::attr(true)) | (x3::lit("-") >> x3::attr(false)))
                          >> add_expr;

    auto const block_expr_def = block_term | simple_expr;
    auto const mult_expr_def  = mult_term | block_expr;
    auto const add_expr_def   = add_term | mult_expr;
    auto const expr_def       = add_expr;

    BOOST_SPIRIT_DEFINE(expr, add_expr, mult_expr, block_expr);
}}

//--------------------------------------------------------------------------------------------------
namespace client { namespace ast
{
    struct printer
    {
        typedef std::string result_type;

        std::string operator()(const expr_ast &ast) const
        {
            return boost::apply_visitor(printer(), ast);
        }
        std::string operator()(const number &value) const
        {
            return boost::apply_visitor(printer(), value);
        }

        std::string operator()(const add_ast &expr) const {
            return "(" + boost::apply_visitor(printer(), expr.lhs) + (expr.add?" + ":" - ")
                   + boost::apply_visitor(printer(), expr.rhs) + ")";
        }

        std::string operator()(const mult_ast &expr) const {
            return "(" + boost::apply_visitor(printer(), expr.lhs) + (expr.mult?" * ":" / ")
                   + boost::apply_visitor(printer(), expr.rhs) + ")";
        }

        std::string operator()(const block_ast &expr) const {
            return boost::apply_visitor(printer(), expr.body);
        }

        std::string operator()(const function &fct) const
        {
            std::string result = fct.name + "(";
            for (std::size_t i = 0; i < fct.params.size(); ++i) {
                result += printer()(fct.params[i]);
                if (i != fct.params.size() - 1)
                    result += ",";
            }
            result += ")";
            return result;
        }

        std::string operator()(int const& value) const
        {
            return std::to_string(value);
        }
        std::string operator()(double const& value) const
        {
            return std::to_string(value);
        }
    };
}}

//--------------------------------------------------------------------------------------------------
int main()
{
    std::vector<std::string> storage = {
        "foo()", "-foo()",
        "f1_2()",
        "foo_bar ()",
        "foo( bar (42, baz()))",
        "foo(5)", "foo(-5)",
        "foo(1.1, foo(4.21e-2, 4., 6))",
        "1.1", "-1.1",
        "1 * 1",
        "foo(1 * 1) * bar(42)",
        "foo(2 + 5.5, bar()*3.4-7)",
        "foo(2 + 5.5, bar(baz(-5/foo())) * 3.4 - 7)",
        "4 + 5 * 6",
        "1+2+3+4*5*6*-7+-8*+9-0",
        "(foo())",
        "foo() * ((1+2)+3*(2+3))",
        "(1+2)*3", "1+2*3",
        "foo"
    };

    using boost::spirit::x3::ascii::space;

    for (const auto &item : storage) {
        using client::parser::expr; // Our grammar
        client::ast::expr_ast ast; // Our tree

        std::string::const_iterator iter = item.begin();
        std::string::const_iterator end = item.end();
        bool r = phrase_parse(iter, end, expr, space, ast);

        if (r && iter == end)
        {
            std::cout << "Ok: " << item << " result: " << client::ast::printer()(ast) << std::endl;
        }
        else
        {
            std::cout << "Fail: " << item << std::endl;
        }
    }
}
