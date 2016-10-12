#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/include/deref.hpp>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>

// Dereference data for associatve containers
#include <boost/fusion/iterator/deref_data.hpp>
#include <boost/fusion/include/deref_data.hpp>

// For pair
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/include/pair.hpp>

// For map
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/container/map/map_fwd.hpp>
#include <boost/fusion/include/map_fwd.hpp>

#include <iostream>
#include <string>

namespace fusion = boost::fusion;

struct Sep {
    Sep(const std::string& n): name(n) {
        std::cout << "Playing with " << name << std::endl;
        std::cout      << "------------------------\n";
    }
    ~Sep() { std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl; }
    std::string name;
};


int main() {
    {
        Sep _("deref");
        int i = 42;
        fusion::vector<int, int&> vec(1, i);
        std::cout << fusion::deref(fusion::begin(vec)) << std::endl;
        std::cout << "Distance = " << fusion::distance(fusion::begin(vec), fusion::end(vec)) << std::endl;
        std::cout << "Next = " << fusion::deref(fusion::next(fusion::begin(vec))) << std::endl;
        std::cout << "Prior = " << fusion::deref(fusion::prior(fusion::end(vec))) << std::endl;
    }
    {
        Sep _("deref_data");
        using map = fusion::map<
            fusion::pair<float, int&>
        >;
        int i = 42;
        map m(
            fusion::make_pair<float, int&>(i)
        );
        std::cout << ++fusion::deref_data(fusion::begin(m)) << std::endl;
        std::cout << ++fusion::deref_data(fusion::begin(m)) << std::endl;
    }
    return 0;
}
