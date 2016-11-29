//http://stackoverflow.com/questions/40477563/boost-spirit-x3-extracting-data-into-x3variant-is-always-empty
#define BOOST_SPIRIT_X3_DEBUG
#include <iostream>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace x3 = boost::spirit::x3;

const x3::rule<class number_tag, long> number = "number";
const auto number_def = x3::long_;
BOOST_SPIRIT_DEFINE(number);

const x3::rule<class string_tag, std::string> string = "string";
const auto string_def = x3::lexeme['"' >> *(x3::char_ - '"') >> '"'];
BOOST_SPIRIT_DEFINE(string);

using any_type = x3::variant<long, std::string>;

const x3::rule<class any_tag, any_type> any = "any";
const auto any_def = number | string;
BOOST_SPIRIT_DEFINE(any);

struct Visitor
{
  using result_type = long;

  result_type operator()(long v) const { return v; }
  result_type operator() (const std::string& v) { return std::atol(v.c_str()); }
};

int main()
{
    const std::string src = "18";
    any_type result;
    auto iter = src.begin();
    bool success = x3::phrase_parse(iter, src.end(), any, x3::space, result);
    if (!success || iter != src.end())
        return 1;
    else {
        Visitor v;
        std::cout << "Result: " << boost::apply_visitor(v, result) << std::endl;
    }
}

