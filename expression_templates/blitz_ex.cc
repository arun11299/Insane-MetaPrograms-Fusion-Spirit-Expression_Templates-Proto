#include <iostream>
#include <type_traits>
#include <array>
#include <vector>
#include <memory>

#define CONTAINER Darray

// Operations
struct plus
{
  static int apply(int a, int b) {
    return a + b;
  }
};

template <typename LeftExpr, typename Operation, typename RightExpr>
struct Expression
{
  Expression(const LeftExpr& l, const RightExpr& r):
    left_(l),
    right_(r)
  {}

  int operator[](size_t idx) const
  {
    return Operation::apply(left_[idx], right_[idx]);
  }

  const LeftExpr& left_;
  const RightExpr& right_;
};

template <typename L, typename R>
auto operator+(const L& l, const R& r)
{
  return Expression<L, plus, R>(l, r);
}

template <typename T, size_t N>
auto operator+(const std::array<T, N>& a, const std::array<T, N>& b)
{
  std::array<T, N> res;
  for (size_t i = 0; i < N; ++i) {
    res [i] = a[i] + b[i];
  }
  return res;
}

template <typename T>
auto operator+(const std::vector<T>& a, const std::vector<T>& b)
{
  std::vector<T> res(a.size());
  for (size_t i  = 0; i < a.size(); ++i) {
    res[i] = a[i] + b[i];
  }
  return res;
}

template <typename T, size_t N>
class Array
{
public:
  Array() = default;
  template <typename U, size_t M, 
  	   typename = typename std::enable_if_t<std::is_convertible<U, T>::value && (M >= N)>>
  Array(const Array<U, M>& other)
  {
    for (size_t i = 0; i < N; ++i) {
      repr_[i] = other[i];
    }
  }
  T& operator[](size_t idx) { return repr_[idx]; }
  T operator[](size_t idx) const { return repr_[idx]; }
  size_t size() const noexcept { return N; }

  template <typename Expr>
  Array& operator=(const Expr& e)
  {
    for (size_t i = 0; i < size(); i++) {
      repr_[i] = e[i];
    }
    return *this;
  }
private:
  T repr_[N];
};


template <typename T>
class Darray
{
public:
  Darray() = default;
  Darray(size_t siz): repr_(new T[siz]) {
    size_ = siz;
  }

  T& operator[](size_t idx) { return repr_[idx]; }
  T operator[](size_t idx) const { return repr_[idx]; }
  size_t size() const noexcept { return size_; }

  template <typename Expr>
  Darray& operator=(const Expr& e)
  {
    for (size_t i = 0; i < size(); i++) {
      repr_[i] = e[i];
    }
    return *this;
  }
private:
  std::unique_ptr<T[]> repr_ = nullptr; 
  size_t size_ = 0;
};


int main() {
  /*
  Array<int, 100000> a, b, c, d;
  for (int i = 0; i < 100; i++) {
    b[i] = i;
    c[i] = i * 2;
    d[i] = i * 3 + b[i] + c[i];
  }

  a = b + c + d;
  std::cout << a[501];
  */

  CONTAINER<uint8_t> a(1000000);
  CONTAINER<uint8_t> b(1000000);
  CONTAINER<uint8_t> c(1000000);
  CONTAINER<uint8_t> d(1000000);

  for (int i = 0; i < a.size(); i++) {
    b[i] = i;
    c[i] = i * 2;
    d[i] = i * 3 + b[i] + c[i];
  }

  a = b + c + d;

  std::cout << static_cast<int>(a[501]);
  return 0;
}
