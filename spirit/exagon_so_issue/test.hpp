#include <boost/spirit/home/x3.hpp>

template<typename Parser>
void test(std::string str, Parser&& p)
{
    auto in = str.begin();
    auto end = str.end();
    bool ret = boost::spirit::x3::phrase_parse(in, end, p, boost::spirit::x3::space);
    ret = (ret && (in == end));
    if(ret)
        std::cout << "success for:\t" << str << std::endl;
    else
         std::cerr << "failed for:\t" << str << std::endl;
        

}
