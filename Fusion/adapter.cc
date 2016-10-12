#include <boost/fusion/adapted.hpp>
#include <boost/fusion/include/adapted.hpp>

#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/iterator.hpp>

#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/include/pair.hpp>

#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>
#include <typeinfo>

namespace fusion = boost::fusion;

struct Sep {
    Sep(const std::string& s): name(s) {
        std::cout << name << '\n';
        std::cout << "-------------------\n";
    }
    ~Sep() {
        std::cout << "~~~~~~~~~~~~~~~~~~" << std::endl;
    }
    std::string name;
};

namespace demo { 
    struct employee {
        std::string name;
        int age;
    };
}

BOOST_FUSION_ADAPT_STRUCT (
    demo::employee,
    (std::string, name),
    (int        , age)
);


BOOST_FUSION_ADAPT_STRUCT_NAMED (
    demo::employee, adapted_employee,
    (std::string, name),
    (int, age)
);

int main() {
    {
        Sep _("C array");
        int arr[3] = {1, 2, 3};
        std::cout << *fusion::begin(arr) << std::endl;
        std::cout << *fusion::next(fusion::begin(arr)) << std::endl;
    }
    {
        Sep _("BOOST_FUSION_ADAPT_STRUCT");
        /*
        fusion::adapted::adapted_employee ae{
                fusion::make_pair<fusion::adapted::adapted_employee::name>("Arun"),
                fusion::make_pair<demo::employee::age>(28)
        };
        */
        using namespace fusion;
        demo::employee ae{"asd", 987};
        std::cout << *fusion::begin(ae) << std::endl;
        std::cout << ae << std::endl;
    }
    
    return 0;
}
