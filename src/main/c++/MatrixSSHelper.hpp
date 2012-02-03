/***************************************************************************
 * MatrixSSHelper.hpp
 *
 *  Copyright 2011 Informed Simplifications, LLC
 *  This file is part of NeuroJet.
 *
 *  NeuroJet is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  NeuroJet is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with NeuroJet.  If not, see <http://www.gnu.org/licenses/lgpl.txt>.
 ****************************************************************************/
#if !defined(MATRIXSSHELPER_HPP)
#define MATRIXSSHELPER_HPP

#include <functional>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <vector>

template <typename T>
class MatrixSSInnerHelper: public std::binary_function<T, T, T> {
 public:
  MatrixSSInnerHelper() { }
  const long double operator() (const long double& avgSoFar,
                                const long double& newVal) {
    return avgSoFar + (newVal * newVal);
  }
};

template <typename T>
class MatrixSSHelper: public std::unary_function<T, void> {
 public:
  MatrixSSHelper(): denominator(0), numerator(0) { }
  void operator() (const std::vector<T>& d) {
    numerator += accumulate(d.begin(), d.end(), T(0), MatrixSSInnerHelper<T>());
    denominator += d.size();
  }
  long double result() const {
    return numerator / denominator;
  }

 private:
  size_t denominator;
  long double numerator;
};

#endif  // MATRIXSSHELPER
