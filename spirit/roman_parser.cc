#include <iostream>
#include <string>
#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;

namespace parser {

namespace numerals {
  // The data associated with each slot is the parsers attribute
  // which is passed to attach semantic actions.
  struct hundreds_ : x3::symbols<unsigned>
  {
    hundreds_()
    {
      add
      	("C"   , 100)
      	("CC"  , 200)
      	("CCC" , 300)
      	("CD"  , 400)
      	("D"   , 500)
      	("DC"  , 600)
      	("DCC" , 700)
      	("DCCC", 800)
      	("CM"  , 900)
      	;
    }
  } hundreds;

  struct tens_ : x3::symbols<unsigned>
  {
    tens_()
    {
      add
        ("X"    , 10)
        ("XX"   , 20)
        ("XXX"  , 30)
        ("XL"   , 40)
        ("L"    , 50)
        ("LX"   , 60)
        ("LXX"  , 70)
        ("LXXX" , 80)
        ("XC"   , 90)
        ;
    }

  } tens;


  struct ones_ : x3::symbols<unsigned>
  {
    ones_()
    {
      add
        ("I"    , 1)
        ("II"   , 2)
        ("III"  , 3)
        ("IV"   , 4)
        ("V"    , 5)
        ("VI"   , 6)
        ("VII"  , 7)
        ("VIII" , 8)
        ("IX"   , 9)
        ;
    }

  } ones;

} // end namespace numerals

using x3::eps;
using x3::lit;
using x3::_val;
using x3::_attr;
using x3::ascii::char_;
using namespace numerals;

auto set_zero = [&](auto& ctx){ _val(ctx) = 0; };
auto add1000  = [&](auto& ctx){ _val(ctx) += 1000; };
auto add      = [&](auto& ctx){ 
  std::cout << "Attr: " << _attr(ctx) << std::endl;
  std::cout << "Val Attr: " << _val(ctx) << std::endl;
  _val(ctx) += _attr(ctx); };

const x3::rule<class roman, unsigned> roman = "roman-rule-parser";
const auto roman_def = 
  eps     [set_zero]
  >>
  (
    -(+lit('M'))  [add1000]
    >> -hundreds  [add]
    >> -tens      [add]
    >> -ones      [add]
  )
  ;

  BOOST_SPIRIT_DEFINE (roman);
}

int main() {
  std::string str;
  unsigned result;

  while (std::getline(std::cin, str))
  {
    if (str.empty() || str[0] == 'q' || str[0] == 'Q') break;

    bool r = parse(std::cbegin(str), std::cend(str), parser::roman, result);
    if (r) {
      std::cout << "Parsing succeeded\n";
      std::cout << "result = " << result << std::endl;
    }
  }
  return 0;
}
