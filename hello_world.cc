#include <iostream>
#include <ostream>
#include <boost/proto/proto.hpp>

using namespace boost;

proto::terminal< std::ostream& >::type cout_ = { std::cout };

template <typename Expr>
void evaluate(const Expr& expr) {
  proto::default_context ctx;
  proto::eval(expr, ctx);
}

int main() {
  evaluate( cout_ << "Hello, " << "World!" );
  return 0;
}
