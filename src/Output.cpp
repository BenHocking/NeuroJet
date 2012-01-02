/***************************************************************************
 * Output.cpp
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
#if !defined(OUTPUT_HPP)
#   include "Output.hpp"
#endif

#include <string>

#if !defined(DATATYPES_HPP)
#   include "DataTypes.hpp"
#endif

std::string Output::CurrentUser = EMPTYSTR;
ostream *Output::StdOut = NULL;
ostream *Output::StdErr = NULL;

ScriptState * ScriptState::ms_curScriptState = NULL;
