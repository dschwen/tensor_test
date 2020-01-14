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
  Tensor(NestedInitializerList<T, 1> t) : _i(0) { initialize(t, 0); }
  Tensor(NestedInitializerList<T, 2> t) : _i(0) { initialize(t, 0); }
  Tensor(NestedInitializerList<T, 3> t) : _i(0) { initialize(t, 0); }
  Tensor(NestedInitializerList<T, 4> t) : _i(0) { initialize(t, 0); }

  // this sadly does not work:
  // template <int D> Tensor(NestedInitializerList<T, D> t) {}

  /// works for order 1 and 2 only
  Tensor<T> transpose();

  // print data for debug purposes
  void printDebug();

protected:
  std::vector<T> _data;
  std::vector<std::size_t> _shape;

private:
  // initializer list processing
  template <typename U>
  void initialize(std::initializer_list<U> u, std::size_t _depth);
  void initialize(std::initializer_list<T> t, std::size_t _depth);
  void checkDimensions(std::size_t size, std::size_t _depth);

  // helper index
  std::size_t _i;
};

template <typename T>
template <typename U>
void
Tensor<T>::initialize(std::initializer_list<U> u, std::size_t _depth)
{
  // outer recursions
  checkDimensions(u.size(), _depth);
  for (auto t : u)
    initialize(t, _depth + 1);
}

template <typename T>
void
Tensor<T>::initialize(std::initializer_list<T> t, std::size_t _depth)
{
  // innermost recursion
  checkDimensions(t.size(), _depth);
  for (const T & v : t)
    _data.push_back(v);
}

template <typename T>
void
Tensor<T>::checkDimensions(std::size_t size, std::size_t _depth)
{
  // check size
  if (_shape.size() <= _depth)
  {
    _shape.resize(_depth + 1);
    _shape[_depth] = size;
  }
  else
  {
    if (_shape[_depth] != size)
      throw std::runtime_error("Inconsistent initializer list size");
  }
}

template <typename T>
void
Tensor<T>::printDebug()
{
  std::cout << "shape:";
  for (auto s : _shape)
    std::cout << ' ' << s;
  std::cout << '\n';

  std::cout << "data: ";
  for (auto d : _data)
    std::cout << ' ' << d;
  std::cout << '\n';
}

int
main()
{
  Tensor<double> a = {1, 2};
  Tensor<double> b = {{1, 2}, {3, 4}};
  Tensor<double> c = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
  Tensor<double> d = {{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}},
                      {{{11, 12}, {13, 14}}, {{15, 16}, {17, 18}}}};

  a.printDebug();
  b.printDebug();
  c.printDebug();
  d.printDebug();

  return 0;
}
