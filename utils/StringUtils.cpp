/***************************************************************************
 * StringUtils.cpp
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

#if !defined(STRINGUTILS_HPP)
#  include "StringUtils.hpp"
#endif

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

namespace neurojet {
  namespace stringutils {
    std::string ltrim(std::string const& source, char const* delims) {
      std::string result(source);
      std::string::size_type index = result.find_first_not_of(delims);
      if (index != std::string::npos)
        result.erase(0, index);
      else
        result.erase();
      return result;
    }

    std::string rtrim(std::string const& source, char const* delims) {
      std::string result(source);
      std::string::size_type index = result.find_last_not_of(delims);
      if (index != std::string::npos)
        result.erase(++index);
      else
        result.erase();
      return result;
    }

    std::string ucase(const std::string &s) {
      std::string toReturn(EMPTYSTR);
      transform(s.begin(), s.end(), back_inserter(toReturn),
                std::ptr_fun<int, int>(toupper));
      return toReturn;
    }

    // Helper function for tokenize. Not used anywhere else
    void addToStrVec(std::vector<std::string>& container, const std::string& s,
                     int begin, int end) {
      // Only adds conditionally
      if (end > begin) {
        container.push_back(s.substr(begin, end - begin));
      }
    }

    /**
     * tokenize separates the string s using token, except where token is inside
     * of elements specified by groups. For example, if groups is ""()[], then
     * the token will be ignored when it is inside quotes, parentheses, or
     * square brackets.
     */
    std::vector<std::string> tokenize(const std::string &s, const char token,
                                      const std::string &groups)
                             throw(std::logic_error) {
      std::vector<std::string> toReturn(0, EMPTYSTR);
      const unsigned int numGrps = groups.size() / 2;
      std::vector<int> grpCnt = std::vector<int>(numGrps, 0);
      const std::string combo = groups + token;
      std::string::size_type prev = -1;
      std::string::size_type start = 0;
      std::string::size_type found = s.find_first_of(combo);
      bool insideGroup = false;
      while (found != std::string::npos) {
        if (s[found] == token) {
          if (!insideGroup) {  // Do NOT combine with previous if statement
            addToStrVec(toReturn, s, start, found);
            start = found + 1;
          }
        } else {
          std::string::size_type grp = groups.find(s[found]);
          int& numInGrp = grpCnt[grp / 2];
          if (grp % 2 == 0) {
            numInGrp++;
          } else {
            numInGrp--;
          }
          insideGroup = (numInGrp > 0);
          if (numInGrp < 0) {
            throw std::logic_error("Mismatching " + groups[grp]);
          }
        }
        prev = found;
        found = s.find_first_of(combo, prev + 1);
      }
      addToStrVec(toReturn, s, start, s.size());
      return toReturn;
    }
  }
}
