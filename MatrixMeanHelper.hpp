#if !defined(MATRIXMEANHELPER_HPP)
#define MATRIXMEANHELPER_HPP

#include <functional>
#include <algorithm>
#include <cstddef>

template <typename T>
class MatrixMeanHelper: public std::unary_function<T, void> {
public:
  MatrixMeanHelper(): denominator(0), numerator(0) { }
  void operator() (const vector<T>& d) {
    numerator += accumulate(d.begin(), d.end(), T(0));
    denominator += d.size();    
  }
  double result() const {
    return static_cast<double>(numerator) / denominator;
  }
private:
  size_t denominator;
  T numerator;
};

#endif // MATRIXMEANHELPER
