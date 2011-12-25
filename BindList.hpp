/***************************************************************************
 * BindList.hpp
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
#if !defined(BINDLIST_HPP)
#   define BINDLIST_HPP

#include <string>
#include <map>
#include <utility>
using std::string;
using std::map;
using std::pair;

template<class T> class BindList {
 public:
  BindList() {}
  ~BindList() {}
  void insert(const string &k, const T &AddObj,
              const bool &IsReadOnly = false) {
    dataMap[k] = pair<T, bool>(AddObj, IsReadOnly);
  }
  inline void remove(const string &k) { dataMap.erase(k); }
  inline bool exists(const string &k) const {
    return (dataMap.find(k) != dataMap.end());
  }
  inline const T & GetEntry(const string &k) const {
    return dataMap.find(k)->second.first;
  }
  inline T & GetEntry(const string &k) {
    return dataMap.find(k)->second.first;
  }
  inline bool IsReadOnly(const string &k) const {
    return dataMap.find(k)->second.second;
  }

 private:
  map<string, pair<T, bool> > dataMap;
};

#endif
