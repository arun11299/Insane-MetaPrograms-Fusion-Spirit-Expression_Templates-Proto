#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>
#include <fstream>

/*
 * Example of a RExpression (limited Json)
 *
 * {
 *   "color" = "blue"
 *   "size" = "29 cm."
 *   "position" = {
 *                  "x" = "123"
 *                  "y" = "456"
 *                }
 * }
 */

namespace client {
namespace ast {
  
  namespace x3 = boost::spirit::x3;
  struct rexpr;

  struct rexpr_value: x3::variant<
                        std::string,
                        x3::forward_ast<rexpr>
                      >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  using rexpr_map = std::map<std::string, rexpr_value>;
  using rexpr_key_value = std::pair<std::string, rexpr_value>;

  struct rexpr {
    rexpr_map entries;
  };

} // end namespace client
} // end namespace ast

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::rexpr,
    (client::ast::rexpr_map, entries)
);

static const int tabsize = 4;

namespace client {
namespace ast {

  struct rexpr_printer
  {
    typedef void result_type;

    rexpr_printer(int indent = 0): indent_(indent) {}

    void operator()(const rexpr& ast) const
    {
      std::cout << '{' << std::endl;

      for (auto const& entry : ast.entries) {
        tab(indent_ + tabsize);
        std::cout << '"' << entry.first << "\" = ";
        boost::apply_visitor(rexpr_printer(indent_ + tabsize), entry.second);
      }

      tab(indent_);
      std::cout << '}' << std::endl;
    }

    void operator()(const std::string& str) const 
    {
      std::cout << '"' << str << '"' << std::endl;
    }

    void tab(int spaces) const
    {
        for (int i = 0; i < spaces; i++) std::cout << ' ';
    }

    int indent_ = 0;
  };

}
}

namespace client {
namespace parser {

  namespace x3 = boost::spirit::x3;
  namespace ascii = x3::ascii;

  using x3::lit;
  using x3::lexeme;

  using ascii::char_;
  using ascii::string;

  // Rules
  x3::rule<struct rexpr_value, ast::rexpr_value>     rexpr_value = "rexpr_value";
  x3::rule<struct rexpr_entry, ast::rexpr_key_value> rexpr_entry = "rexpr_entry";
  x3::rule<struct rexpr      , ast::rexpr>           rexpr       = "rexpr";

  // Parsers
  const auto quoted_string =
    lexeme['"' >> *(char_ - '"') >> '"'];

  const auto rexpr_value_def = 
    quoted_string | rexpr;

  const auto rexpr_entry_def =
    quoted_string >> '=' >> rexpr_value;

  const auto rexpr_def =
    '{' >> *rexpr_entry >> '}';

  BOOST_SPIRIT_DEFINE(rexpr_value, rexpr_entry, rexpr);
}
}


int main() {
  std::ifstream in("rexpr1.txt", std::ios::in);

  if (!in) {
    std::cerr << "Failed to open file" << std::endl;
    return 1;
  }

  in.unsetf(std::ios::skipws); // No white space skipping!
  std::string file_data;

  std::copy(
      std::istream_iterator<char>(in),
      std::istream_iterator<char>(),
      std::back_inserter(file_data)
  );

  using client::parser::rexpr;
  client::ast::rexpr ast;

  using boost::spirit::x3::ascii::space;

  bool r = phrase_parse(file_data.begin(), file_data.end(),
      (rexpr), space, ast);

  if (r) {
    std::cout << "Parsing Succeded !!!" << std::endl;
    client::ast::rexpr_printer printer;
    printer(ast);
    std::cout << "---------------------" << std::endl;
  } else {
    std::cout << "Parsing Failed" << std::endl;
  }
  return 0;
}
