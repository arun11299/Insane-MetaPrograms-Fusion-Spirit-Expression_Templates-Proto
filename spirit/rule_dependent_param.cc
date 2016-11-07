#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <vector>

namespace x3 = boost::spirit::x3;



namespace parsing {

    struct rule_class;
    x3::rule<rule_class, std::vector<int>> parser = "parser";

    struct MyParser : x3::parser<MyParser> {
      template <typename Iter, typename Ctx, typename TTT, typename ATTR>
      bool parse(Iter& iFirst, const Iter& iLast, const Ctx& iCtx, TTT&, ATTR& oAttr) const {
        std::cout << "Whaaaatttt!!" << std::endl;
        return x3::as_parser(x3::int_ % x3::lit(',')).parse(iFirst, iLast, parsing::parser, iCtx, oAttr);
      }
    };

    auto const my_parser = MyParser{};

    //struct separator {};
    char separator(',');

    //auto parser_def = x3::int_ % x3::lit(x3::get<separator>(/* context */)); // candidate function template not viable: requires single argument 'context'
    auto parser_def = my_parser;

    BOOST_SPIRIT_DEFINE(parser)
}

void parse(const std::string &data, const char separator) {
    using namespace std;

    //auto parser = x3::with<parsing::separator>(ref(separator)) [parsing::parser] >> x3::eoi;
    auto parser = parsing::parser >> x3::eoi;

    if (x3::parse(data.begin(), data.end(), parser))
        cout << "Parse succeeded\n";
    else
        cout << "Parse failed\n";
}

int main() {
    parse("1 2 3", ' ');
    parse("1,2,3", ',');
    parse("1;2;3", ';');
}
