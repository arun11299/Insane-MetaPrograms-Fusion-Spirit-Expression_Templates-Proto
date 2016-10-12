#include <iostream>
#include <type_traits>

template <typename... T>
using void_t = void;

template <typename T, typename = void_t<>> struct has_name : std::false_type {};

template <typename T>
struct has_name <T, void_t<typename T::name>> : std::true_type {};

int main() {
    std::cout << has_name<int>::value << std::endl;
    return 0;
}
