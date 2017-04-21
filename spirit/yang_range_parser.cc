#include <iostream>
#include <vector>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>


namespace x3 = boost::spirit::x3;

namespace range_parser { namespace ast {

  using Range = x3::variant<
                  std::pair<int64_t, int64_t>
                >;

  template <typename T>
  using all_ranges = std::vector<std::pair<T, boost::optional<T>>>;
}}

namespace range_parser {

  namespace x3 = boost::spirit::x3;
  namespace ascii = x3::ascii;

  using x3::lit;
 
  using x3::double_;
  using x3::long_long;
  using x3::uint_;

  x3::rule<struct expression_int,   std::pair<int64_t, boost::optional<int64_t>>>     expression_int;
  x3::rule<struct range_expression, ast::all_ranges<int64_t>> range_expression;
  //x3::rule<struct expression_uint, ast::Range<int64_t>> expression_uint;

  const auto expression_int_def = 
    x3::lexeme[
                (x3::long_long >> -(".." >> x3::long_long))
              ]
    ;

  const auto range_expression_def = -expression_int % '|';

  BOOST_SPIRIT_DEFINE (expression_int, range_expression);
}


int main() {
  using boost::spirit::x3::ascii::space;
  using range_parser::range_expression;

  std::string input{"10 | 9"};
  range_parser::ast::all_ranges<int64_t> ranges;

  bool res = phrase_parse(
        std::begin(input), std::end(input),
        range_expression,
        space,
        ranges
      );

  if (res) {
    int64_t prev_max = 0;
    for (auto& elem : ranges) {
      //range_parser::printer::range_printer prnt;
      if (elem.first < prev_max) {
        std::cout << "Wrong!\n";
        return 1;
      }
      prev_max = elem.first;
      if (elem.second) {
        if (elem.second.get() < prev_max) {
          std::cout << "Wrong again!!\n";
          return 1;
        }
        prev_max = elem.second.get();
      }
    }
  } else {
    std::cout << "Parsing Failed \n";
  }

  return 0;
}
