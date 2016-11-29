#ifndef KYLE_TYPES_DEF_HPP
#define KYLE_TYPES_DEF_HPP

#include "types.hpp"


namespace client { namespace parser {
namespace x3 = boost::spirit::x3;


typedef x3::rule<struct lambda_type_class, ast::LambdaType> lambda_type_type;


const class_type_type class_type = "class_type";
const lambda_type_type lambda_type = "lambda_type";
const type_type type = "type";

auto const identifier = +x3::alnum;

auto const type_def =
        (lambda_type | class_type);

auto const lambda_type_def =
        ("(" > -(type % ",") > ")" > "=>" > type)
        | x3::repeat(1)[class_type] >> "=>" > type;


auto const class_type_def =
        (identifier % "::") >> -("<" > type % "," > ">");


BOOST_SPIRIT_DEFINE(
        lambda_type,
        class_type,
        type
);


}}


#endif //KYLE_TYPES_DEF_HPP
