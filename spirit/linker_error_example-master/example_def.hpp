#ifndef LINKER_ERROR_EXAMPLE_EXAMPLE_DEF_HPP
#define LINKER_ERROR_EXAMPLE_EXAMPLE_DEF_HPP

#include "ast.hpp"
#include "example.hpp"
#include "types.hpp"

namespace client { namespace parser {

namespace { const auto& type = client::type(); }

const var_dec_type var_dec = "var_dec";

auto const var_dec_def = x3::lexeme["var "]
                         > +x3::alnum
                         > ":"
                         > type //<- HERE WHEN I CHANGE > type to >> type to get linker error
                         > ";";

BOOST_SPIRIT_DEFINE(var_dec)

}}

#endif //LINKER_ERROR_EXAMPLE_EXAMPLE_DEF_HPP
