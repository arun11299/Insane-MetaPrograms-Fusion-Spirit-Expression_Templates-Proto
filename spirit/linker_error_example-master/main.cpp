#include <iostream>
#include "example_def.hpp"

template<typename Parser, typename Attribute>
bool test(const std::string& str, Parser&& p, Attribute&& attr)
{
    using iterator_type = std::string::const_iterator;
    iterator_type in = str.begin();
    iterator_type end = str.end();

    bool ret = boost::spirit::x3::phrase_parse(in, end, p, boost::spirit::x3::ascii::space, attr);
    ret &= (in == end);
    return ret;

}

int main()
{
    client::ast::VariableDec attr;

    std::cout << test("var foo : ;", client::var_dec() , attr);
    return 0;
}
