#include <iostream>
#include <string>
#include <cassert>
#include <type_traits>
#include <typeinfo>
#include <typeindex>

#include <boost/hana.hpp>
#include <boost/any.hpp>

struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

namespace hana = boost::hana;

namespace sw_case {
  
  template <typename T>
  auto case_ = [](auto f) {
    return hana::make_pair(hana::type_c<T>, f);
  };

  struct default_t;
  auto default_ = case_<default_t>;

  template <typename Any, typename Default>
  auto process(Any& a, const std::type_index&, Default& default_)
  {
    return default_();
  }

  template <typename Any, typename Default, typename Case, typename... Rest>
  auto process(Any& a, const std::type_index& t, Default& def, 
      Case& case_, Rest&... rest)
  {
    using T = typename decltype(+hana::first(case_))::type;
    return t == typeid(T) ? hana::second(case_)(*boost::unsafe_any_cast<T>(&a))
      : process(a, t, def, rest...);
  }

  // Switch takes in a any object
  // and returns a callable which takes
  // variadic list of case_ types

  template <typename Any>
  auto switch_(Any& a)
  {
    return [&](auto... cases_) {
      auto cases = hana::make_tuple(cases_...);

      // Check whether default is there
      auto default_ = hana::find_if(cases, [](const auto& c) {
            return hana::first(c) == hana::type_c<default_t>;
          });

      static_assert (default_ != hana::nothing, "Default case not provided");

      auto rest = hana::filter(cases, [](const auto& c) {
            return hana::first(c) != hana::type_c<default_t>;
          });

      return hana::unpack(rest, [&](auto&... rest) {
            return process(a, a.type(), hana::second(*default_), rest...);
          });
    };
  }
  
}

int main() {
  {
    using namespace hana::literals;

    auto animals = hana::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});
    auto anm = animals[1_c];
    assert (anm.name == std::string("Garfield"));
  }
  {
    // Type computation
    auto animal_types = hana::make_tuple(hana::type_c<Fish*>, hana::type_c<Cat&>, hana::type_c<Dog>);

    auto no_pointers = hana::remove_if(animal_types, [](auto a) {
            return hana::traits::is_pointer(a);
          });

    static_assert(no_pointers == hana::make_tuple(hana::type_c<Cat&>, hana::type_c<Dog>), "");
  }
  {
    //Introspection
    struct Person {
      BOOST_HANA_DEFINE_STRUCT(
          Person,
          (std::string, name),
          (int, age)
      );
    };

    auto serialize = [](std::ostream& os, const auto& object) {
      hana::for_each(hana::members(object), [&](const auto& member) {
            os << member << std::endl; 
          });
    };

    Person p;
    p.name = "Arun M";
    p.age = 28;
    serialize (std::cout, p);
  }
  {
    using namespace sw_case;
    boost::any c = 'x';
    auto res = switch_(c)(
          case_<int>([](auto i)->int {std::cout << "int\n"; return i;} ),
          case_<char>([](auto c)->char {std::cout << "char\n"; return c;} ),
          default_([]()->int{std::cout << "unknown\n"; return 42;})
        );
  }
  return 0;
}
