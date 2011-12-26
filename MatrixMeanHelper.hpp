/***************************************************************************
 * MatrixMeanHelper.hpp
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
