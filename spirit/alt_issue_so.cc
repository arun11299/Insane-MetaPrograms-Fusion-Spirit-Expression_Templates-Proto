#include <iostream>

//#define BOOST_SPIRIT_X3_DEBUG
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <string>
#include <list>

namespace ast
{
    struct identifer {
        char                    name;
    };
    struct expression {
        struct chunk {
            char                operator_;
            ast::identifer      identifer;
        };
        ast::identifer          identifer;
        std::list<chunk>        chunk_list;
    };
}

BOOST_FUSION_ADAPT_STRUCT(ast::identifer,
    name
)
BOOST_FUSION_ADAPT_STRUCT(ast::expression::chunk,
    operator_, identifer
)
BOOST_FUSION_ADAPT_STRUCT(ast::expression,
    identifer, chunk_list
)

namespace parser
{
    namespace x3 = boost::spirit::x3;

    auto const identifier = x3::rule<struct _, char> { "identifier" } =
        x3::char_("0-9");
    auto const operator_1 = x3::rule<struct _, char> { "operator.ABC" } =
        x3::char_("ABC");
    auto const operator_2 = x3::rule<struct _, char> { "operator.XYZ" } =
        x3::char_("XYZ");

    template<typename T>
    auto as = [](auto p) { return x3::rule<struct _, T>{ "as" } = x3::as_parser(p); };

    auto const expression_chunk_1 = x3::rule<struct _, std::list<ast::expression::chunk>> { "expression.1" } =
        *as<ast::expression::chunk>(operator_1 > identifier)
        ;
    auto const expression_chunk_2 = x3::rule<struct _, std::list<ast::expression::chunk>> { "expression.2" } =
        x3::repeat(1)[as<ast::expression::chunk>(operator_2 > identifier)]
        ;
    auto const expression_chunk_3 = x3::rule<struct _, std::list<ast::expression::chunk>> { "expression.3" } =
        (expression_chunk_1 | expression_chunk_2)
        ;
    auto const expression = x3::rule<struct _, ast::expression> { "expression" } =
        identifier >> expression_chunk_3
        ;
}

struct visitor {
    visitor(std::ostream& os) : os{ os } { }
    void operator()(ast::expression const& node) {
        os << "(";
        (*this)(node.identifer);
        for(auto const& chunk : node.chunk_list) {
            os << "(" << chunk.operator_ << " ";
            (*this)(chunk.identifer);
            os << ")";
        }
        os << ")\n";
    }
    void operator()(ast::identifer const& node) {
        os << "(" << node.name << ")";
    }
    std::ostream& os;
};


int main()
{
    namespace x3 = boost::spirit::x3;

    for(std::string const str: {
        "1 X 2",        // expected: ((1)(X (2)))
        "3 A 4 A 5",    // expected: ((3)(A (4))(A (5)))
        "6 X 7 X 8"     // has to fail
    }) {
      auto iter = str.begin(), end = str.end();

      ast::expression attr;
      bool r = x3::phrase_parse(iter, end, parser::expression, x3::space, attr);

      std::cout << "parse '" << str << "': ";
      if (r && iter == end) {
        std::cout << "succeeded:\n";
        //visitor(std::cout)(attr);

      } else {
        std::cout << "*** failed ***\n";
      }
    }

    return 0;
}
