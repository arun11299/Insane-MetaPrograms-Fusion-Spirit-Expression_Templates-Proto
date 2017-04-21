#include <iostream>
#include <type_traits>

namespace inspect {
namespace detail {
    
    template <typename T>
    class construct
    {
    public:
        constexpr construct() = default;
        
        template <typename Params>
        constexpr auto operator()(Params&&  args)
        {
            return check<Params>(int{});
        }
    private:
       template <typename Params>
       constexpr auto check(int) 
                -> decltype(std::declval<T>()(std::declval<Params>()), std::true_type())
       {
           std::cout << "wdafadfs\n\n";
           return {};
       }
       
       template <typename Params>
       constexpr std::false_type check(...)
       {
           std::cout << "qwertysd\n\n";
           return {};
       }
    };
    
}

template <typename F>
constexpr auto is_valid(F&& f) 
{
    return detail::construct<F>();
}

} // end namespace inspect

namespace meta
{
    template <typename A, typename B>
    constexpr auto if_(std::true_type, A a, B b)
    {
        return a;
    }
    
    template <typename A, typename B>
    constexpr auto if_(std::false_type, A a, B b)
    {
        return b;
    }
} // end namespace meta

auto has_serialize = inspect::is_valid( [](auto&& type) ->decltype(type.serialize()) {} );

struct Data {
    void serialize() const { std::cout << "Data::Serialize\n"; }
};

template <typename T>
constexpr void serialize(const T& obj)
{
    meta::if_(has_serialize(obj),
              [] (auto x) { x.serialize(); },
              [] (auto x) { std::cout << "No serialize\n"; })(obj);                             
}


int main() {
    serialize(Data{});
    return 0;
}
