#if !defined(MATRIXSUMHELPER_HPP)
#define MATRIXSUMHELPER_HPP

#include <functional>
#include <numeric>

template <typename T>
class MatrixSumHelper: std::binary_function<T, vector<T>, T> {
public:
  const T operator()(const T sumSoFar, const vector<T> toAccum) {
    return sumSoFar + accumulate(toAccum.begin(), toAccum.end(), T(0));
  }
};
#endif // MATRIXSUMHELPER_HPP
