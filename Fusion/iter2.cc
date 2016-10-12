#include <iostream>
#include <string>
#include <tuple>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

struct Window {
    void show() { std::cout << "Window::show" << std::endl; }
};

struct Door {
    void show() { std::cout << "Door::show" << std::endl; } 
};

struct Widget {
    void show() { std::cout << "Widget::show" << std::endl; }
};

int main() {
    Window w;
    Door d;
    Widget w2;

    boost::fusion::for_each(
        std::tie(w, d, w2),
        [](auto&& t) { t.show(); }
    );
    return 0;
}
