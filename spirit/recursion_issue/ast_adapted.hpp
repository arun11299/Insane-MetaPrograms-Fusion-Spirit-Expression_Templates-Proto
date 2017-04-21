#ifndef AST_ADAPTED_HPP
#define AST_ADAPTED_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_TPL_STRUCT(
  (T),
  (ast::operation_sequence_entry)(T),
  (std::string, op),
  (T, expr)
);

BOOST_FUSION_ADAPT_STRUCT(
  ast::unary_expression,
  (ast::union_expression, expr)
);

BOOST_FUSION_ADAPT_STRUCT(
  ast::eq_expression,
  (ast::unary_expression, lhs_expr),
  (std::vector<typename ast::eq_expression::expr_seq_type>, rhs_expr)
);

BOOST_FUSION_ADAPT_STRUCT(
  ast::and_expression,
  (ast::eq_expression, lhs_expr),
  (std::vector<typename ast::and_expression::expr_seq_type>, rhs_expr)
);

BOOST_FUSION_ADAPT_STRUCT(
  ast::or_expression,
  (ast::and_expression, lhs_expr),
  (std::vector<typename ast::or_expression::expr_seq_type>, rhs_expr)
);

BOOST_FUSION_ADAPT_STRUCT(
  ast::function_call,
  (std::string, func_name),
  (std::vector<ast::or_expression>, args)
);

BOOST_FUSION_ADAPT_STRUCT(
  ast::filter_expression,
  (ast::primary_expression, prim_expr),
  (std::vector<x3::forward_ast<ast::or_expression>>, predicates)
);


#endif
