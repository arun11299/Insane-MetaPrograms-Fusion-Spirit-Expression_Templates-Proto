#include <iostream>
#include <string>
#include <vector>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3.hpp>

#include "hold.hpp"

namespace x3 = boost::spirit::x3;

namespace ast {

  struct name_t {
    std::string name;
  };

}

BOOST_FUSION_ADAPT_STRUCT (
    ast::name_t,
    (std::string, name),
);

namespace grammar {
  using x3::ascii::string;
  using x3::ascii::char_;
  using x3::lexeme;

  template<typename T>
  auto as = [](auto p) { return x3::rule<struct _, T>{} = as_parser(p); };

  x3::rule<struct name, ast::name_t> gname = "name parser";

  auto ncname = lexeme[+(char_ - ':')];
  auto qname = ncname;

  auto gname_def = qname;

  BOOST_SPIRIT_DEFINE (gname);
}


int main() {
  using grammar::gname;
  using boost::spirit::x3::ascii::space;

  ast::name_t p;
  std::string input {"prefix"};

  bool res = phrase_parse(std::begin(input), std::end(input), gname, space, p);
  if (!res) {
    std::cerr << "Failed to parse!\n";
    return 1;
  }

  std::cout << p.name << std::endl;
  return 0;
}

