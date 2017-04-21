#ifndef REC_EXPR_DEF_HPP
#define REC_EXPR_DEF_HPP

#include "rec_expr.hpp"
#include "ast.hpp"
#include "ast_adapted.hpp"

namespace x3 = boost::spirit::x3;

namespace parser {
  // Bring in the spirit parsers
  using x3::lexeme;
  using x3::alpha;
  using x3::alnum;
  using x3::ascii::char_;
  using x3::ascii::string;
  using x3::lit;
  using x3::ascii::digit;
  using x3::int_;
  using x3::uint_;
  using x3::double_;

  template<typename T>
  auto as = [](auto p) { return x3::rule<struct _, T>{} = as_parser(p); };

  auto str_ = [](const char* lit) { return x3::string(lit); };

  x3::rule<class path_expr, ast::path_expression> path_expr = "path-expr";

  using or_expr_type = x3::rule<class or_expr, ast::or_expression>;
  const or_expr_type or_expr = "or-expr";

  const auto ncname = x3::rule<class ncname, std::string>{"ncname"}
              = x3::lexeme[+(char_ - ':')]
              ;

  const auto qname = x3::rule<class qname, std::string>{"qname"}
             = as<std::string>(ncname >> char_(':') >> ncname)
             | as<std::string>(ncname)
             ;

  const auto union_expr = x3::rule<class union_expr, ast::union_expression>{"union-expr"}
                  = path_expr % '/'
                  ;

  const auto unary_expr = x3::rule<class unary_expr, ast::unary_expression>{"unary-expr"}
                  = -x3::lit('-') >> union_expr
                  ;

  const auto equality_expr = x3::rule<class eq_expr, ast::eq_expression>{"equality-expr"}
                     =  unary_expr
                     >> *(as<ast::operation_sequence_entry<ast::unary_expression>>
                          ( (str_("=") | str_("!=")) > unary_expr )
                         )
                     ;

  const auto and_expr = x3::rule<class and_expr, ast::and_expression>{"and-expr"}
                =  equality_expr
                >> *(as<ast::operation_sequence_entry<ast::eq_expression>>
                     ( str_("and") > equality_expr )
                    )
                ;

  const auto or_expr_def =  and_expr 
                         >> *(as<ast::operation_sequence_entry<ast::and_expression>>
                              ( str_("or") >> and_expr )
                             )
                         ;

  const auto function_name = as<std::string>(qname);

  const auto function_arg = or_expr;

  const auto function_call = x3::rule<class func_call, ast::function_call>{"func-call"}
                     = function_name > '(' > (or_expr % ',') > ')'
                     ;

  const auto prim_expr = x3::rule<class prim_expr, ast::primary_expression>{"prim-expr"}
                 = ('$' > qname)
                 | ('"' > *(char_ - '"') > '"')
                 | ('(' > or_expr > ')')
                 | (int_ | uint_ | double_)
                 | function_call
                 ;

  
  const auto predicate = '[' > or_expr > ']';

  const auto filter_expr = x3::rule<class filter_expr, ast::filter_expression>{"filter-expr"}
                   = prim_expr >> *(predicate)
                   ;

  const auto path_expr_def = -(filter_expr) >> -(lit("/") | lit("//")) >> -(prim_expr);

  BOOST_SPIRIT_DEFINE (path_expr);
  BOOST_SPIRIT_DEFINE (or_expr);
}

const parser::or_expr_type& or_expression() {
  return parser::or_expr;
}

#endif
