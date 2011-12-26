/***************************************************************************
 * StringUtils.hpp
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
#define STRINGUTILS_HPP
#include <functional>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace neurojet {
  namespace stringutils {
    // Creates an empty string in a way that should be compiler independent.
    // See http://www.newartisans.com/2009/10/a-c-gotcha-on-snow-leopard.html
    const std::string EMPTYSTR = std::string(0, '\0');

    std::string ltrim(std::string const& source, char const* delims=" \t\r\n");

    std::string rtrim(std::string const& source, char const* delims=" \t\r\n");

    std::string ucase(const std::string &s);

    // Helper function for tokenize. Not used anywhere else
    void addToStrVec(std::vector<std::string>& container, const std::string& s,
                     int begin, int end);

    std::vector<std::string> tokenize(const std::string &s, const char token,
                                      const std::string &groups)
                             throw(std::logic_error);

    template<typename T>
    inline std::string to_string(const T& data_in) {
      std::ostringstream os;
      os << std::setprecision(20) << data_in;
      return os.str();
    }

    template<>
    inline std::string to_string(const std::vector<std::string>& data_in) {
      std::ostringstream os;
      os << std::setprecision(20);
      for (unsigned int i = 0; i < data_in.size(); i++) {
        if (i > 0) {
          os << " ";
        }
        os << data_in.at(i);
      }
      return os.str();
    }

    template<>
    inline std::string to_string(const std::vector<double>& data_in) {
      std::ostringstream os;
      os << std::setprecision(20);
      for (unsigned int i = 0; i < data_in.size(); i++) {
        if (i > 0) {
          os << " ";
        }
        os << data_in.at(i);
      }
      return os.str();
    }

    template<>
    inline std::string to_string(const std::vector<int>& data_in) {
      std::ostringstream os;
      os << std::setprecision(20);
      for (unsigned int i = 0; i < data_in.size(); i++) {
        os << data_in.at(i);
      }
      return os.str();
    }

    template<typename T>
    inline T from_string(const std::string &data_in)
      throw(std::logic_error) {
      if (data_in[0] == '.')
        return from_string<T>("0" + data_in);
      std::istringstream is(data_in);
      T data_out;
      is >> data_out;
      return data_out;
    }

    template<>
    inline std::vector<std::string> from_string(const std::string &data_in)
      throw(std::logic_error) {
      std::vector<std::string> data_out = tokenize(data_in, ' ', "()[]{}");
      return data_out;
    }

    template<>
    inline std::vector<double> from_string(const std::string &data_in)
      throw(std::logic_error) {
      std::vector<std::string> tokens = tokenize(data_in, ' ', "");
      std::vector<double> data_out(tokens.size());
      for (unsigned int i = 0; i < tokens.size(); ++i) {
        data_out[i] = from_string<double>(tokens[i]);
      }
      return data_out;
    }

    template<>
    inline std::vector<int> from_string(const std::string &data_in)
      throw(std::logic_error) {
      std::vector<std::string> tokens = tokenize(data_in, ' ', "");
      std::vector<int> data_out(tokens.size());
      for (unsigned int i = 0; i < tokens.size(); ++i) {
        data_out[i] = from_string<int>(tokens[i]);
      }
      return data_out;
    }
  }
}

#endif  // STRINGUTILS_HPP
