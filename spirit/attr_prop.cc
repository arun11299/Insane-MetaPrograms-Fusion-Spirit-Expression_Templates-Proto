#include <iostream>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/io.hpp>

namespace x3 = boost::spirit::x3;

namespace ast {

  struct Expr : x3::variant<
                  std::pair<int, int>,
                  int
                >
  {
    using base_type::base_type;
    using base_type::operator=;
  };
}

namespace parser {

  const x3::rule <struct one, std::pair<int, int>> one = "one";
  const x3::rule <struct two_, ast::Expr> two = "two";

  auto action = [](auto &x) {};

  const auto one_def = x3::int_ >> " " >> x3::int_;
  const auto two_def = one[action] | x3::int_[action];

  BOOST_SPIRIT_DEFINE(one, two);
}

int main() {
  std::string str("123");
  auto r = parse(str.begin(), str.end(), parser::two);
  return 0;
}
