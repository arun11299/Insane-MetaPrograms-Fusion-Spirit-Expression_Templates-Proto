//#define BOOST_SPIRIT_X3_DEBUG
#include <iostream>
#include <boost/spirit/home/x3.hpp>
#include "parser.hpp"
#include "test.hpp"
namespace x3 = boost::spirit::x3;
int main(){
    test("foo.bar.baz[bar.foo][bar]", parser::expression);
    test("foo(bar.bar)[baz]", parser::expression);
    /*
    test("foo(bar)(baz)", parser::expression);
    test("foo(bar, baz)[baz]", parser::expression);
    test("foo[bar](baz)[bam]", parser::expression);
    test("foo.foo()", parser::expression);
    test("foo(bar[baz], foo)", parser::expression);
    */
}
