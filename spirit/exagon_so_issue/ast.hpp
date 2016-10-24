#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <vector>

namespace ast{
    namespace x3 = boost::spirit::x3;

    enum class operator_t
    {
      _eq_,       // ==
      _ne_,       // !=
      _lt_,       // <
      _gt_,       // >
      _le_,       // <=
      _ge_,       // >=
      _add_,      // +
      _sub_,      // -
      _mul_,      // *
      _div_,      // /
      _mod_,      // %
      _pos_,      // unary +
      _neg_,      // unary -
      _not_,      // unary !
      _size_,     // unary #
      _bitnot_,   // unary ~
      _bitand_,   // &
      _bitor_,    // |
      _bitxor_,   // ^
      _shl_,      // <<
      _shr_,      // >>
      _and_,      // &&
      _or_,       // ||
      _idx_,      // []
      _apply_,    // ()
      _access_    // .
    };


    struct nil{};
    struct Expression;

    typedef x3::variant<
            nil,
            std::string,
            x3::forward_ast<Expression>
            //std::vector<Expression> //adding this as an operand for function parameter
    > Operand;
    
    struct Operation {
        operator_t operator_;
        Operand operand_;
    };
    
    struct Expression {
        Operand first_;
        std::vector<Operation> rest_;
    };
}
BOOST_FUSION_ADAPT_STRUCT(ast::Operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::Expression, first_, rest_)
