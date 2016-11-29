#include <iostream>
#include <vector>
#include <string>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/io.hpp>

namespace x3 = boost::spirit::x3;

namespace ast {
  struct MyExpr : x3::variant<
                    std::pair<std::string, std::string>,
                    std::string
                  >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct expression {
    MyExpr expr_;
  };
}

BOOST_FUSION_ADAPT_STRUCT(
      ast::expression,
      (ast::MyExpr, expr_)
);


namespace parser {

  const x3::rule<struct pair_rule, std::pair<std::string, std::string>> pair_expression = "pair_expression";
  const x3::rule<struct my_rule_p, ast::expression> expression = "expression";

  auto action = [](auto& ctx) {
    std::cout << "Called Action" << std::endl;
  }; 
  auto action2 = [](auto& ctx) {
    std::cout << "Called Action2" << std::endl;
  }; 

  const auto pair_expression_def = x3::alnum >> '=' >> x3::alnum;
  const auto expression_def = pair_expression[action2];

  BOOST_SPIRIT_DEFINE(pair_expression, expression);
}

int main() {
  std::string s("1=2");
  auto itr = s.begin();
  bool r = parse(itr, s.end(), parser::expression);
  if (not r or itr != s.end()) std::cout << "Failed" << std::endl;
  return 0;
}
