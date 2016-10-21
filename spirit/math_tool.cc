#define BOOST_SPIRIT_X3_DEBUG
#include <iostream>
#include <string>
#include <fstream>
#include <list>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
//#include <boost/fusion/include/io.hpp>

namespace math_tool {
namespace ast {
  namespace x3 = boost::spirit::x3;

  struct none {};
  struct expression;

  struct operand : x3::variant<
                    none,
                    double,
                    int,
                    x3::forward_ast<expression>
                   >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct operation {
    char operator_;
    operand operand_;
  };

  struct expression {
    operand operand_;
    std::list<operation> operations_;
  };

}} // END namespaces

BOOST_FUSION_ADAPT_STRUCT(
    math_tool::ast::expression,
    (math_tool::ast::operand, operand_),
    (std::list<math_tool::ast::operation>, operations_)
);

BOOST_FUSION_ADAPT_STRUCT(
    math_tool::ast::operation,
    (char, operator_),
    (math_tool::ast::operand, operand_)
);

namespace math_tool {
namespace parser {

  namespace x3 = boost::spirit::x3;
  namespace ascii = x3::ascii;

  using x3::lit;
  using x3::alnum;
  using x3::lexeme;

  using x3::double_;
  using x3::int_;
  using x3::char_;

  x3::rule<struct expression,    ast::expression > expression    = "expression";
  x3::rule<struct primary_expr,  ast::operand    > primary_expr  = "primary_expr";
  x3::rule<struct additive_expr, ast::expression > additive_expr = "additive_expr";

  const auto additive_expr_def =
    primary_expr
    >> *(  char_('+') > primary_expr )
    ;

  const auto primary_expr_def =
      double_
    | int_
    | '(' > expression > ')'
    ;

  const auto expression_def = additive_expr;

  BOOST_SPIRIT_DEFINE(expression, primary_expr, additive_expr);

}} // END namespaces


int main() {
  std::string expr = "(1 + 2)";

  using boost::spirit::x3::ascii::space;
  using math_tool::parser::expression;

  math_tool::ast::expression ast;

  bool r = phrase_parse(
        expr.begin(), expr.end(),
        (expression), space, ast
      );

  if (r) {
    std::cout << "Parsing succeded!!" << std::endl;
  } else {
    std::cout << "Maann! it failed..." << std::endl;
  }
  return 0;
}
