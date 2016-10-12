#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/include/at.hpp>

#include <iostream>
#include <string>

namespace fusion = boost::fusion;

int main() {
    fusion::vector<int, std::string> v(0, "Arun");
    std::cout << fusion::at_c<0>(v) << std::endl;
    std::cout << fusion::at_c<1>(v) << std::endl;
    return 0;
}
