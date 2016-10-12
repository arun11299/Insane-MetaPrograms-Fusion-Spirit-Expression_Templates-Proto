#include <iostream>
#include <string>
#include <complex>
#include <boost/spirit/home/x3.hpp>

template <typename Iter>
bool parse_complex(Iter start, Iter end, std::complex<double>& c)
{
  using boost::spirit::x3::phrase_parse;
  using boost::spirit::x3::double_;
  using boost::spirit::x3::_attr;
  using boost::spirit::x3::ascii::space;

  double rN, iN;

  auto rF = [&rN](auto& ctx) { rN = _attr(ctx); };
  auto iF = [&iN](auto& ctx) { iN = _attr(ctx); };

  bool res = phrase_parse(
	start, end,
	(           // Grammar begin
	  '(' >> double_[rF]
	  >> -(',' >> double_[iF]) >> ')'
	)           // Grammar end
	, space
      );

  c = std::complex<double>(rN, iN);
  return res;
}

int main() {
  std::string str;
  while (getline(std::cin, str)) {
    if (!str.length() || str[0] == 'q' || str[0] == 'Q') {
      break;
    }
    std::complex<double> c;
    if (parse_complex(std::begin(str), std::end(str), c)) {
      std::cout << "Parsing succeeded: " << c << std::endl;
    } else {
      std::cout << "Parsing Failed!!" << std::endl;
    }
  }
  return 0;
}
