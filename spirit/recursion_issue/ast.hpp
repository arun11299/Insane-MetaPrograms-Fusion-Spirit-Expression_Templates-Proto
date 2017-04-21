#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace x3 = boost::spirit::x3;

namespace ast {

  struct or_expression;
  struct function_call;

  template <typename T>
  struct operation_sequence_entry
  {
    std::string op;
    T expr;
  };

  struct primary_expression: x3::variant<
                              std::string,
                              x3::forward_ast<or_expression>,
                              std::string,
                              int32_t,
                              uint32_t,
                              double,
                              x3::forward_ast<function_call>
                             >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct filter_expression
  {
    primary_expression prim_expr;
    std::vector<x3::forward_ast<or_expression>> predicates;
  };

  struct path_expression: x3::variant<
                            boost::optional<filter_expression>,
                            boost::optional<primary_expression>
                          >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  using union_expression = std::vector<path_expression>;

  struct unary_expression
  {
    union_expression expr;
  };

  struct eq_expression
  {
    using expr_seq_type = operation_sequence_entry<unary_expression>;
    unary_expression lhs_expr;
    std::vector<expr_seq_type> rhs_expr;
  };

  struct and_expression
  {
    using expr_seq_type = operation_sequence_entry<eq_expression>;
    eq_expression lhs_expr;
    std::vector<expr_seq_type> rhs_expr;
  };

  struct or_expression
  {
    using expr_seq_type = operation_sequence_entry<and_expression>;
    and_expression lhs_expr;
    std::vector<expr_seq_type> rhs_expr;
  };

  struct function_call
  {
    std::string func_name;
    std::vector<or_expression> args;
  };
}


#endif
