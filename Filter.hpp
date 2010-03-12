#if !defined(FILTER_HPP)
#  define FILTER_HPP

#  include <vector>
#  include <numeric>
#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif
#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
#include <stdexcept>
using std::length_error;

class Filter {
public:
   // initialize to filter of size 1 and value 1
  Filter(): m_filterValue(1, 1.0f) { }
  Filter(int size): m_filterValue(size, 1.0f) { }
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
     return inner_product(m_filterValue.begin(), m_filterValue.end(), historicalData.begin(), 0.0);
  }
  inline void setFilter(const DataList& filterVals) {
    if (filterVals.empty()) throw length_error("Attempted to create empty filter");
    m_filterValue = filterVals;
  }
  inline const DataList& getFilter() const { return m_filterValue; }
  inline unsigned int size() const { return m_filterValue.size(); }
private:
  DataList m_filterValue;
};

#endif
