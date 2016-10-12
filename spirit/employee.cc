#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <algorithm>
#include <iostream>
#include <string>

namespace client {
namespace ast {

    struct employee {
      int age;
      std::string first_name;
      std::string last_name;
      double salary;
    };

    using boost::fusion::operator<<;

} // End namespace ast
} // End namespace client

BOOST_FUSION_ADAPT_STRUCT(
  client::ast::employee,
  (int        , age),
  (std::string, first_name),
  (std::string, last_name),
  (double     , salary)
);

namespace client {
namespace parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::int_;
    using x3::lit;
    using x3::double_;
    using x3::lexeme;
    using ascii::char_;

    const x3::rule<class employee, client::ast::employee> employee = "employee";

    // Parse quoted strings without any whitespace. lexme takes
    // care of parsing input without any whitespace
    const auto quoted_string = lexeme[ '"' >> +(char_ - '"') >> '"'];

    const auto employee_def = 
        lit("employee")
        >> '{'
        >> int_          >> ','
        >> quoted_string >> ','
        >> quoted_string >> ','
        >> double_
        >> '}'
        ;

    BOOST_SPIRIT_DEFINE (employee);

} // End namespace client
} // End namespace parser

int main() {
    using boost::spirit::x3::ascii::space;
    using client::parser::employee;

    std::string str;
    while (std::getline(std::cin, str)) {
        client::ast::employee emp;
        auto beg = std::cbegin(str);
        auto end = std::cend(str);

        bool r = phrase_parse(beg, end, employee, space, emp);
        if (r) {
            std::cout << "----------------------------------\n";
            std::cout << "Parsing Succeded!\n";
            std::cout << boost::fusion::tuple_open('[');
            std::cout << boost::fusion::tuple_close(']');
            std::cout << boost::fusion::tuple_delimiter(", ");
            std::cout << "got: " << emp << std::endl;
            std::cout << "----------------------------------\n";
        } else {
            std::cout << "Parsing Failed!!" << std::endl;
            std::cout << std::distance(beg, end) << std::endl;
            break;
        }
    }
    return 0;
}
