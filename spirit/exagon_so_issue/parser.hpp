#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"
#include "operators.hpp"
namespace parser{
    namespace x3 = boost::spirit::x3;
    
    
    template<typename T>
    auto as = [](auto p) { return x3::rule<struct _, T>{} = as_parser(p); };
    
    
    typedef x3::rule<struct multiplicative_expr_class, ast::Expression> multiplicative_expr_type;
    typedef x3::rule<struct expression_class, ast::Expression> expression_type;
    typedef x3::rule<struct primary_expr_class, ast::Operand> primary_expr_type;
    typedef x3::rule<struct func_call_call_class, ast::Expression> func_call_expr_type;
    typedef x3::rule<struct unary_expr_class, ast::Operand> unary_expr_type;

    
    
    const primary_expr_type primary_expr = "primary_expr";    
    const func_call_expr_type func_call_expr = "func_call_expr";
    const expression_type expression = "expression";
    const multiplicative_expr_type multiplicative_expr = "multiplicative_expr";
    const unary_expr_type unary_expr = "unary_expr";

   
    auto const primary_expr_def =
        +(x3::alpha | x3::char_('.'))
        | ( "(" > expression > ")" );
    
    auto const idx = as<ast::Operation>(helper::idxaccess_op > primary_expr > ']');
    auto const func = as<ast::Operation>(helper::func_call_op > primary_expr%',' > ')');
    auto const data = as<ast::Operation>(helper::access_op > primary_expr);

    auto const func_call_expr_def =
        primary_expr >> *( idx | func | data );

    auto const unary_expr_def =
              func_call_expr
                      | as<ast::Operation>(helper::unary_op > func_call_expr);
    
    auto const multiplicative_expr_def =
        primary_expr >>  *(idx | func);

    auto const expression_def = multiplicative_expr_def;
    
    
BOOST_SPIRIT_DEFINE(primary_expr,
                    func_call_expr,
                    multiplicative_expr,
                    unary_expr,
                    expression);

}
