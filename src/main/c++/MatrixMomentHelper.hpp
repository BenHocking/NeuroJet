/***************************************************************************
 * MatrixMomentHelper.hpp
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
#if !defined(MATRIXMOMENTHELPER_HPP)
#define MATRIXMOMENTHELPER_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <vector>

template <typename T>
class MatrixMomentInnerHelper: public std::binary_function<T, T, T> {
 public:
  MatrixMomentInnerHelper(T a, double m): avg(a), moment(m) { }
  const long double operator() (const long double& avgSoFar,
                                const long double& d) {
    return avgSoFar + pow(d - avg, moment);
  }

 private:
  const long double avg;
  const long double moment;
};

template <typename T>
class MatrixMomentHelper: public std::unary_function<T, void> {
 public:
  MatrixMomentHelper(T a, double m)
    : denominator(0), numerator(0), avg(a), moment(m) { }
  void operator() (const std::vector<T>& d) {
    numerator += accumulate(d.begin(), d.end(), T(0),
                            MatrixMomentInnerHelper<T>(avg, moment));
    denominator += d.size();
  }
  long double result() const {
    return numerator / denominator;
  }

 private:
  size_t denominator;
  long double numerator;
  const long double avg;
  const long double moment;
};

#endif  // MATRIXMOMENTHELPER
