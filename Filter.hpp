/***************************************************************************
 * Filter.hpp
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
#if !defined(FILTER_HPP)
#  define FILTER_HPP

#  include <vector>
#  include <numeric>
#  include <stdexcept>
#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif
#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
using std::length_error;

class Filter {
 public:
  // initialize to filter of size 1 and value 1
  Filter(): m_filterValue(1, 1.0f) { }
  explicit Filter(int size): m_filterValue(size, 1.0f) { }
  Filter(const Filter& f): m_filterValue(f.m_filterValue) { }
  ~Filter() { }
  Filter& operator=(const Filter& f) {
    if (this != &f) {   // make sure not same object
      m_filterValue = f.m_filterValue;
    }
    return *this;        // Return ref for multiple assignment
  }
  // historicalData has most recent data up front (push_front(), pop_back())
  inline float apply(const DataList& historicalData) const {
     return inner_product(m_filterValue.begin(), m_filterValue.end(),
                          historicalData.begin(), 0.0);
  }
  inline void setFilter(const DataList& filterVals) {
    if (filterVals.empty()) {
      throw length_error("Attempted to create empty filter");
    }
    m_filterValue = filterVals;
  }
  inline const DataList& getFilter() const { return m_filterValue; }
  inline unsigned int size() const { return m_filterValue.size(); }

 private:
  DataList m_filterValue;
};

#endif
