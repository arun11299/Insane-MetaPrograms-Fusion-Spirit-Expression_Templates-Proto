//#define BOOST_SPIRIT_X3_DEBUG
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <typeinfo>
#include <cassert>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/io.hpp>

/*
 * First, define the AST
 */
namespace math_tool {
namespace ast {
  namespace x3 = boost::spirit::x3;

  struct none {};
  // Fwd decl the expression structure
  struct expression;
  struct function_call;

  /*
   * The operand can either be a double or the expression
   * itself (recursive gramar)
   */
  struct operand : x3::variant<
                    none,
                    double,
                    x3::forward_ast<expression>,
                    x3::forward_ast<function_call>
                   >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  /*
   * structure to define the operation on an operand
   * which is an operator followed by the operand variant.
   */
  struct operation {
    char operator_;
    operand operand_;
  };

  /*
   * The expression structure which is an operand
   * followed by a chain of operations (The operation itself
   * may have sub expressions)
   */
  struct expression {
    operand operand_;
    std::list<operation> operations_;
  };

  struct function_call {
    std::string function_name_;
    std::list<expression> args_;
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

BOOST_FUSION_ADAPT_STRUCT(
    math_tool::ast::function_call,
    (std::string, function_name_),
    (std::list<math_tool::ast::expression>, args_)
);

namespace math_tool {
namespace parser {

  namespace x3 = boost::spirit::x3;
  namespace ascii = x3::ascii;

  using x3::lit;
  using x3::alpha;
  using x3::alnum;
  using x3::lexeme;

  using x3::double_;
  using x3::char_;

  x3::rule<struct expression,          ast::expression   > expression          = "expression";
  x3::rule<struct primary_expr,        ast::operand      > primary_expr        = "primary_expr";
  x3::rule<struct additive_expr,       ast::expression   > additive_expr       = "additive_expr";
  x3::rule<struct multiplicative_expr, ast::expression   > multiplicative_expr = "multiplicative_expr";
  x3::rule<struct function_call_expr,  ast::function_call> function_call_expr  = "function_call_expr";
  x3::rule<struct argument_list_expr,  std::list<ast::expression>> arg_list_expr = "arg_list_expr";

  const auto additive_expr_def =
    multiplicative_expr 
    >> *( (char_('+') > multiplicative_expr)
        | (char_('-') > multiplicative_expr)
        )
    ;

  const auto multiplicative_expr_def =
    primary_expr
    >> *( (char_('*') > primary_expr)
        | (char_('/') > primary_expr)
        )
    ;

  const auto primary_expr_def =
      double_
    //| function_call_expr
    | '(' > expression > ')'
    ;

  const auto arg_list_expr_def = expression % ',';

  const auto function_call_expr_def =
    lexeme[(alpha | '_') >> *(alnum | '_')]
    >> -('(' > arg_list_expr > ')')
    ;

  const auto expression_def = additive_expr;

  BOOST_SPIRIT_DEFINE(expression, 
                      primary_expr, 
                      additive_expr,
                      multiplicative_expr, 
                      function_call_expr, 
                      arg_list_expr
                      );

}} // END namespaces

namespace math_tool {
namespace ast {

  /*
   * This is a visitor struct which does the evaluation
   * of the parsed expression tree in a variant.
   */
  struct Evaluator
  {
    typedef double result_type;

    double operator()(const expression& expr) const
    {
      double r = boost::apply_visitor(*this, expr.operand_);
      for (const auto& oper : expr.operations_) {
        r = (*this)(oper, r);
      }

      return r;
    }

    double operator()(const operation& oper, double left) const
    {
      double right = boost::apply_visitor(*this, oper.operand_);

      switch (oper.operator_) {
      case '+':
        return left + right;
      case '-':
        return left - right;
      case '*':
        return left * right;
      case '/':
        return left / right;
      default:
        assert (0);
      };
    }

    double operator()(const function_call&) const
    {
      return 42;
    }

    double operator()(double value) const
    {
      return value;
    }

    double operator()(none) const
    {
      return 0;
    }

  };

}}


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
    math_tool::ast::Evaluator eval;
    auto r = eval(ast);
    std::cout << "Result = " << r << std::endl;
    assert (r == 3);
  } else {
    std::cout << "Maann! it failed..." << std::endl;
  }

  expr = "(4 + (2 + 10) - 1)";
  math_tool::ast::expression ast2;
  r = phrase_parse(
      expr.begin(), expr.end(),
      (expression), space, ast2
      );

  if (r) {
    std::cout << "Parsing succeded!!" << std::endl;
    math_tool::ast::Evaluator eval;
    auto r = eval(ast2);
    std::cout << "Result = " << r << std::endl;
    assert (r == 15);
  } else {
    std::cout << "Maann! it failed..." << std::endl;
  }

  expr = "4 + (2 * 6)/2";
  math_tool::ast::expression ast3;
  r = phrase_parse(
      expr.begin(), expr.end(),
      (expression), space, ast3
      );

  if (r) {
    std::cout << "Parsing succeded!!" << std::endl;
    math_tool::ast::Evaluator eval;
    auto r = eval(ast3);
    std::cout << "Result = " << r << std::endl;
    assert (r == 10);
  } else {
    std::cout << "Maann! it failed..." << std::endl;
  }
  
  return 0;
}
