/***************************************************************************
 * MatrixSumHelper.hpp
 *
 *  Copyright 2011 LevyLab
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
#if !defined(MATRIXSUMHELPER_HPP)
#define MATRIXSUMHELPER_HPP

#include <functional>
#include <numeric>
#include <vector>

template <typename T>
class MatrixSumHelper: std::binary_function<T, std::vector<T>, T> {
 public:
  const T operator()(const T sumSoFar, const std::vector<T> toAccum) {
    return sumSoFar + accumulate(toAccum.begin(), toAccum.end(), T(0));
  }
};
#endif  // MATRIXSUMHELPER_HPP
