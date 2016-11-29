//
// Created by lukas on 11.11.16.
//

#ifndef LINKER_ERROR_EXAMPLE_AST_HPP_HPP
#define LINKER_ERROR_EXAMPLE_AST_HPP_HPP

#include <boost/fusion/adapted/struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <vector>
#include <string>

namespace client { namespace ast {

namespace x3 = boost::spirit::x3;

struct LambdaType;
struct ClassType;
struct nil{};

typedef x3::variant <
    nil,
    x3::forward_ast<LambdaType>,
    x3::forward_ast<ClassType>
> Type;

struct LambdaType {
    std::vector<Type> parameters_;
    Type return_type_;
};

struct ClassType {
    std::vector<std::string> name_;
    std::vector<Type> template_args_;
};

struct VariableDec {
    std::string _name;
    Type _type;
};


}}

BOOST_FUSION_ADAPT_STRUCT(client::ast::LambdaType, parameters_, return_type_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::ClassType, name_, template_args_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::VariableDec, _name, _type)

#endif //LINKER_ERROR_EXAMPLE_AST_HPP_HPP
