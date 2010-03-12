#if !defined(MATRIXSSHELPER_HPP)
#define MATRIXSSHELPER_HPP

#include <functional>
#include <algorithm>
#include <cstddef>
#include <cmath>

template <typename T>
class MatrixSSInnerHelper: public std::binary_function<T, T, T> {
public:
  MatrixSSInnerHelper() { }
  const T operator() (const T& avgSoFar, const T& newVal) {
    return avgSoFar + (newVal * newVal);
  }
};

template <typename T>
class MatrixSSHelper: public std::unary_function<T, void> {
public:
  MatrixSSHelper(): denominator(0), numerator(0) { }
  void operator() (const vector<T>& d) {
    numerator += accumulate(d.begin(), d.end(), T(0), MatrixSSInnerHelper<T>());
    denominator += d.size();    
  }
  T result() const {
    return numerator / denominator;
  }
private:
  size_t denominator;
  T numerator;
};

#endif // MATRIXSSHELPER
