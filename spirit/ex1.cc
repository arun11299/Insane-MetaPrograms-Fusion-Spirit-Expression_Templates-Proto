#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <string>
#include <list>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace ast {
  struct rec_impl;

  struct rec_ast: x3::variant<
                    int,
                    x3::forward_ast<rec_impl>
                  >
  {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct rec_impl
  {
    std::pair<rec_ast, std::string> expr_;
  };

  using wth = std::list<std::string>;

}


namespace parser {
  using x3::lexeme;
  using x3::lit;
  using x3::ascii::char_;
 
  using my_attr = std::pair<boost::optional<std::string>, std::string>;
 
  x3::rule<class ncname_tok, std::string> ncname    = "ncname-token";
  x3::rule<class qname_tok, my_attr> qname = "qname";
  x3::rule<class wth, ::ast::wth> my_expr = "my-expr";
 
  auto ncname_def = lexeme[*(char_ - ':')];
  auto qname_def = -( ncname >> ':' ) >> ncname;

  // REcursive grammar
  x3::rule<class rec, ast::rec_ast> rec = "rec";


  auto rec_def = x3::int_
               | ( rec >> '>' >> ncname )
               ;

  auto my_expr_def = x3::rule<class wth, ::ast::wth>{"my-expr"}
                   = ncname % '|';
  
  BOOST_SPIRIT_DEFINE (ncname, qname, rec, my_expr);
}

int main() {
  {
    std::string input("just-go");
    std::string::iterator strbegin = input.begin();

    parser::my_attr p;

    x3::parse(strbegin, input.end(),
            parser::qname,
            p);

    if (p.first) {
      std::cout << "There should be no prefix: " << p.first << std::endl;
    }

    std::cout << "local name: " << p.second << std::endl;

    std::cout << "-------------rec-test---------------\n";
  }
  {
    ast::wth h;
    std::string input{"abc | def"};
    bool ret = phrase_parse(input.begin(), input.end(),
                            parser::my_expr,
                            x3::space,
                            h
                            );
  }

  return 0;
}
