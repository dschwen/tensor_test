#include <initializer_list>
#include <vector>
#include <iostream>

// nested initializer list

template <typename T, std::size_t depth>
struct NestedInitializerListTempl
{
  using type = std::initializer_list<typename NestedInitializerListTempl<T, depth - 1>::type>;
};

template <typename T>
struct NestedInitializerListTempl<T, 0>
{
  using type = T;
};

template <typename T, std::size_t depth>
using NestedInitializerList = typename NestedInitializerListTempl<T, depth>::type;

// tensor class

template <typename T>
class Tensor
{
public:
  /// zero scalar
  Tensor() : _data(1, T(0.0)), _shape(1, 1) {}

  /// scalar with value t
  Tensor(T t);

  /// up to order four tensors, convenience constructors
  Tensor(NestedInitializerList<T, 1> t) { initialize(t, 0); }
  Tensor(NestedInitializerList<T, 2> t) { initialize(t, 0); }
  Tensor(NestedInitializerList<T, 3> t) { initialize(t, 0); }
  Tensor(NestedInitializerList<T, 4> t) { initialize(t, 0); }

  // this sadly does not work:
  // template <int D> Tensor(NestedInitializerList<T, D> t) {}

  /// works for order 1 and 2 only
  Tensor<T> transpose();

protected:
  // initializer list processing
  template <typename U>
  void initialize(std::initializer_list<U> u, std::size_t _depth);
  void initialize(std::initializer_list<T> t, std::size_t _depth);

  std::vector<T> _data;
  std::vector<std::size_t> _shape;
};

template <typename T>
template <typename U>
void
Tensor<T>::initialize(std::initializer_list<U> u, std::size_t _depth)
{
  // outer recursions
}

template <typename T>
void
Tensor<T>::initialize(std::initializer_list<T> t, std::size_t _depth)
{
  // innermost recursion
}

int
main()
{
  Tensor<double> a = {1, 2};
  Tensor<double> b = {{1, 2}, {3, 4}};
  Tensor<double> c = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
  Tensor<double> d = {{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}},
                      {{{11, 12}, {13, 14}}, {{15, 16}, {17, 18}}}};
  return 0;
}
