#include <iostream>
#include <string>
#include <vector>
#include <boost/spirit/home/x3.hpp>

#include "rec_expr_def.hpp"

namespace x3 = boost::spirit::x3;

int main() {
  using x3::space;
  using parser::or_expr;

  ast::or_expression oexpr;
  std::string input = "$ab/$cd or $ef";

  bool res = phrase_parse(input.begin(),
                          input.end(),
                          or_expr,
                          space,
                          oexpr);

  if (!res) {
    std::cout << "Parsing failed miserably!\n";
    return 1;
  }
  return 0;
}
