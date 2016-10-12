#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <string>

int main() {
  std::string input("(1.0, 2.0)");
  std::string::iterator strbegin = input.begin();
  std::pair<double, double> p;
  x3::parse(strbegin, input.end(),
          '(' >> boost::spirit::x3::double_ >> ", " >> boost::spirit::x3::double_ >> ')', // parser grammar
              p);                                               // attribute to fill while parsing

  return 0;
}
