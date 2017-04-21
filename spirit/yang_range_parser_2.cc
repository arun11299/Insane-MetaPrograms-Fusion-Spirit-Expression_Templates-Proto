#include <iostream>
#include <vector>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>


namespace x3 = boost::spirit::x3;

namespace range_parser {

// Represents a single range
template <typename T>
using range_element_t = std::pair<T, boost::optional<T>>;

// For storing parsed ranges
template <typename T>
using range_list_t = std::vector<range_element_t<T>>;

// The parsing rule
namespace x3 = boost::spirit::x3;

using x3::lit;
using x3::long_long;  // For int64_t
using x3::ulong_long; // For uint64_t

// Rule to parse signed integer type expression
x3::rule<struct expression_int,  range_element_t<int64_t>>  expression_int;
// Rule to parse unsigned integer type expression
x3::rule<struct expression_uint, range_element_t<uint64_t>> expression_uint;
// Rule to parse multiple range expressions of signed integer
x3::rule<struct range_expression_int, range_list_t<int64_t>> range_expression_int;
// Rule to parse multiple range expressions of unsigned integer
x3::rule<struct range_expression_uint, range_list_t<uint64_t>> range_expression_uint;

const auto expression_int_def =
  x3::lexeme[
              (x3::long_long >> -(".." >> x3::long_long))
            ]
  ;

const auto expression_uint_def =
  x3::lexeme[
              (x3::ulong_long >> -(".." >> x3::ulong_long))
            ]
  ;

const auto range_expression_int_def  = -expression_int  % '|';
const auto range_expression_uint_def = -expression_uint % '|';


BOOST_SPIRIT_DEFINE (expression_int, range_expression_int, expression_uint, range_expression_uint);

}


int main() {
  using boost::spirit::x3::ascii::space;
  using range_parser::range_expression_int;

  std::string input{"10 | 9"};
  range_parser::range_list_t<int64_t> ranges;

  bool res = phrase_parse(
        std::begin(input), std::end(input),
        range_expression_int,
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
