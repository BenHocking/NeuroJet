/***************************************************************************
 * Matlab.hpp
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
#if !defined(MATLAB_HPP)
#  define MATLAB_HPP

#  include <string>

class MatlabCommand {
 public:
  MatlabCommand(std::string LHS, std::string RHS):
    m_LHS(LHS), m_RHS(RHS) {}
  std::string getLHS() const {return m_LHS;}
  std::string getRHS() const {return m_RHS;}

 private:
  // LHS is left hand side of (first) equal sign
  // RHS is right hand side of equal sign
  // If there is no equal sign, then only the RHS exists
  std::string m_LHS;
  std::string m_RHS;
};
#endif
