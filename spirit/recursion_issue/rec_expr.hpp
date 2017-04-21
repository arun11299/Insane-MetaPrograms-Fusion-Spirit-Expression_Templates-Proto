#ifndef REC_EXPR_HPP
#define REC_EXPR_HPP

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace parser {

  namespace x3 = boost::spirit::x3;

  using or_expr_type = x3::rule<class or_expr, ast::or_expression>;

  BOOST_SPIRIT_DECLARE (or_expr_type);
}

const parser::or_expr_type& or_expression();

#endif
