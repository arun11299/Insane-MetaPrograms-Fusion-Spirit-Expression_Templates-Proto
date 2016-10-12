#include <iostream>
#include <cassert>
#include <boost/proto/proto.hpp>

using namespace boost;

struct arg1_tag {};

proto::terminal<arg1_tag>::type arg1 = {};

/*
 * Lambda here is both a EDSL grammer and a function object
 * It inherits a call operator which works on the passed
 * expression.
 * The call operator makes use of the Base class template parameters
 * i.e `proto::when_` and `proto::otherwise_` to decide what to
 * do when encountered with a particular Expression token
 */

struct Lambda
  : proto::or_<
      // When evaluating a terminal placeholder, return the state
      proto::when< proto::terminal<arg1_tag>, proto::_state >
      // Other wise do the default thing
    , proto::otherwise< proto::_default< Lambda > >
  >
{};

// This is pseudo C++ language is
// auto Lambda(expr, state = null) {
//  if (decltype(expr) == terminal<arg1_tag>) {
//    return _state(expr, state);
//  } else {
//    return _default(expr, state, Lambda)
//  }
// }
/*
auto _default( expr, state = null, eval = _default )
{
    // Dispatch to the correct case based on the
    // node type of the expression
    switch ( tag_type( expr ) )
    {
    case terminal:
        return value( expr );
 
    case plus:
        return eval( left( expr ), state, eval ) + eval( right( expr ), state, eval );
 
    case minus:
        return eval( left( expr ), state, eval ) - eval( right( expr ), state, eval );
 
    // ... handle all the operators similarly
    }
}
*/


int main() {
  int i = Lambda()(arg1 + 42, 1);
  // 1 is the state here
  // Above expands to
  // default(expr -> arg1 42 +) => left(expr) = arg1 ; right(expr) = 42
  // left(expr) = eval ( arg1, state = 1 ) which returns 1 itself
  // right(expr) = eval (42, state) = Lambda()(42, 1) = default(42) = 42
  
  assert (i == 43);

  return 0;
}
