#include <iostream>
#include <string>

#include <boost/fusion/view.hpp>
#include <boost/fusion/include/view.hpp>

#include <boost/fusion/container.hpp>
#include <boost/fusion/include/container.hpp>

#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>

namespace fusion = boost::fusion;

struct Sep {
    Sep(const std::string& n): name(n) {
        std::cout << name << '\n';
        std::cout << "----------------\n";
    }
    ~Sep() {
        std::cout << "~~~~~~~~~~~~~~~~" << std::endl;
    }
    std::string name;
};

int main() {
    {
        Sep _("single_view");
        fusion::single_view<int> view(3);
        std::cout << view << std::endl;
    }
    {
        Sep xx("filter_view");
        using boost::mpl::_;
        using boost::mpl::not_;
        using boost::is_class;

        typedef fusion::vector<std::string, char, long, bool, double> vector_type;

        vector_type v("a-string", '@', 987654, true, 6.6);
        fusion::filter_view<vector_type const, not_<is_class<_> > > view(v);
        std::cout << view << std::endl;
    }
    {
        Sep _("joint_view");
        fusion::vector<std::string, int> v1("1", 2);
        fusion::vector<std::string, int> v2("3", 4);

        fusion::joint_view<
                fusion::vector<std::string, int>,
                fusion::vector<std::string, int>
            > jv(v1, v2);
        std::cout << jv << std::endl;

        fusion::list<char, float> l('A', 4.2f);
        fusion::joint_view<
                decltype(jv),
                fusion::list<char, float>
            > jv2(jv, l);
        std::cout << jv2 << std::endl;

    }
    return 0;
}
