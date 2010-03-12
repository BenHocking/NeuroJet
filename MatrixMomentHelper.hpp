#if !defined(MATRIXMOMENTHELPER_HPP)
#define MATRIXMOMENTHELPER_HPP

#include <functional>
#include <algorithm>
#include <cstddef>
#include <cmath>

template <typename T>
class MatrixMomentInnerHelper: public std::binary_function<T, T, T> {
public:
  MatrixMomentInnerHelper(T a, double m): avg(a), moment(m) { }
  const T operator() (const T& avgSoFar, const T& d) {
    return avgSoFar + pow(d - avg, moment);
  }
private:
  const T avg;
  const double moment;
};

template <typename T>
class MatrixMomentHelper: public std::unary_function<T, void> {
public:
  MatrixMomentHelper(T a, double m): denominator(0), numerator(0), avg(a), moment(m) { }
  void operator() (const vector<T>& d) {
    numerator += accumulate(d.begin(), d.end(), T(0), MatrixMomentInnerHelper<T>(avg, moment));
    denominator += d.size();    
  }
  T result() const {
    return numerator / denominator;
  }
private:
  size_t denominator;
  T numerator;
  const T avg;
  const double moment;
};

#endif // MATRIXMOMENTHELPER
