#include <boost/spirit/home/x3.hpp>

namespace parser {
namespace helper {

  namespace x3 = boost::spirit::x3;


  struct access_op_t : x3::symbols<ast::operator_t>
  {
    access_op_t()
    {
        add(".", ast::operator_t::_access_);
    }
  } const access_op;//!< object of the \ref access_op_t parser struct

  struct idxaccess_op_t : x3::symbols<ast::operator_t>
  {
    idxaccess_op_t()
    {
        add("[", ast::operator_t::_idx_);
    }
  } const idxaccess_op;//!< object of the \ref idxaccess_op_t parser struct

  struct func_call_op_t : x3::symbols<ast::operator_t>
  {
    func_call_op_t()
    {
        add("(", ast::operator_t::_idx_);
    }
  } const func_call_op;//!< object of the \ref func_call_op_t parser struct

  struct unary_op_t : x3::symbols<ast::operator_t>
  {
    unary_op_t()
    {
        add("+", ast::operator_t::_pos_)
           ("-", ast::operator_t::_neg_)
           ("!", ast::operator_t::_not_)
           ("#", ast::operator_t::_size_)
           ("~", ast::operator_t::_bitnot_);


    }
  } const unary_op;//!< object of the \ref unary_op_t parser struct


  struct multiplicative_op_t : x3::symbols<ast::operator_t>
  {
    multiplicative_op_t()
    {
        add("*", ast::operator_t::_mul_)
           ("/", ast::operator_t::_div_)
           ("%", ast::operator_t::_mod_);
    }
  } const multiplicative_op;//!< object of the \ref multiplicative_op_t parser struct

}
}
