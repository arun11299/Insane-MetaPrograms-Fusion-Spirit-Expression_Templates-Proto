#include <iostream>
#include <string>
#include <typeinfo>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>

// For pair
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/include/pair.hpp>

// For map
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/container/map/map_fwd.hpp>
#include <boost/fusion/include/map_fwd.hpp>

namespace fusion = boost::fusion;

struct print_xml
{
  template <typename T>
  void operator()(const T& x) const
  {
    std::cout <<
       '<' << typeid(x).name() << ">\n"
       << '\t' << x << '\n' <<
       "</" << typeid(x).name() << ">\n"
    ;
  }
};

// Fusion's map associate types with elements. It can be used as a cleverer replacement of the struct. Example:
namespace fields {
    struct name;
    struct age;
};

struct map_dumper {
    template <typename Pair>
    void operator()(const Pair& p) const {
        std::cout << p.second << std::endl;
    }
};

struct Seperator {
    Seperator(const std::string& n): name(n) {
        std::cout << "Playing with " << n << std::endl;
        std::cout << "------------------------------" << std::endl;
    }
    ~Seperator() {
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    }
    std::string name;
};

int main() {
    {
        Seperator _("Vector");
        fusion::vector<int, char, std::string> stuff{1, 'x', "howdy"};
        int i = fusion::at_c<0>(stuff);
        char c = fusion::at_c<1>(stuff);
        std::string str = fusion::at_c<2>(stuff);
        std::cout << i << " : " << c << " : " << str << std::endl;

        fusion::for_each(stuff, print_xml());
    }
    {
        Seperator _("Maps");
        using person = fusion::map<
            fusion::pair<fields::name, std::string>, 
            fusion::pair<fields::age, int>
        >;
        person a_person(
                    fusion::make_pair<fields::name>("Arun"),
                    fusion::make_pair<fields::age>(28)
                );

        fusion::for_each(a_person, map_dumper());
    }

    return 0;
}
