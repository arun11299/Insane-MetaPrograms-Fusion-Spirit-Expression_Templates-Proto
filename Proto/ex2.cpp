#include <iostream>
#include <boost/proto/proto.hpp>

using namespace boost;

struct integer_ {};
boost::proto::terminal<integer_>::type integer = {};

// Transforms
proto::_default<> eval;

int main() {
  //boost::proto::display_expr( (map["place"]="heaven and earth", map["thing"]="philosophy") );
  boost::proto::display_expr( (integer("a") = integer("10") + "20") );
  boost::proto::display_expr (integer + 42);

  auto r = eval( proto::lit(1) + 42 );
  return 0;
}
