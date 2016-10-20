#define BOOST_SPIRIT_X3_DEBUG
#include <iostream>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace ast{
    struct LambdaType;
    struct ClassType;
    struct nil{};

    typedef x3::variant<
        nil,
        x3::forward_ast<LambdaType>,
        x3::forward_ast<ClassType>
    > Type;

    struct LambdaType {
        std::vector<Type> parameters_;
        Type return_type_;
    };

    struct ClassType{
        std::vector<std::string> name_; 
        std::vector<Type> template_args_;
    };
}

BOOST_FUSION_ADAPT_STRUCT (ast::LambdaType, parameters_, return_type_);
BOOST_FUSION_ADAPT_STRUCT (ast::ClassType,  name_,       template_args_);

namespace parser{

    typedef x3::rule<struct one_arg_lambda_type_class, ast::LambdaType> one_arg_lambda_type_type;
    typedef x3::rule<struct lambda_type_class        , ast::LambdaType> lambda_type_type;
    typedef x3::rule<struct class_type_class         , ast::ClassType>  class_type_type;
    typedef x3::rule<struct type_class               , ast::Type>       type_type;

    const class_type_type          class_type          = "class_type";
    const lambda_type_type         lambda_type         = "lambda_type";
    const one_arg_lambda_type_type one_arg_lambda_type = "one_arg_lambda_type";
    const type_type                type_p              = "type";

    auto const type_p_def = class_type | one_arg_lambda_type | lambda_type;

    auto const lambda_type_def =
        "(" >> -(type_p % ",") >> ")" >> "=>" >> type_p;
        
    auto const  one_arg_lambda_type_def = x3::repeat(1)[type_p%","] >> x3::lit("=>") >> type_p;

    auto const identifier = +x3::alnum;

    auto const class_type_def =
            (identifier % "::") >> -("<" >> type_p%"," >> ">");

    BOOST_SPIRIT_DEFINE(
            one_arg_lambda_type,
            lambda_type,
            class_type,
            type_p
    );
}

int main()
{
    std::string input = "(foo, foo) => foo"; 
    /*
    to test:
    foo => foo 
    () => foo
    (foo)=> foo
    foo
    foo<foo=>bar>
    */
    x3::phrase_parse(input.begin(), input.end(), parser::type_p, x3::space);
}

