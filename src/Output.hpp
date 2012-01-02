/***************************************************************************
 * Output.hpp
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
#   define OUTPUT_HPP

#   if !defined(BORLAND)
#      if !defined(WIN32)
#         define GOOD_COMPILER
#      endif
#   endif

#define CALL_ERROR Output::Err() << "ERROR\n\n"
#define ERR_WHERE ScriptState::getLocation() << std::endl << std::endl

#   include <string>
#   include <sstream>
#   include <ostream>
using std::ostream;
using std::string;
using std::ostringstream;

#   if !defined(DATATYPES_HPP)
#      include "DataTypes.hpp"
#   endif

////////////////////////////////////////////////////////////////////////////////
// Some script parsing functions
////////////////////////////////////////////////////////////////////////////////

class StackNode {
 public:
  StackNode()
    : m_str(EMPTYSTR), m_line(0), m_next(NULL) { }
  StackNode(const string& curData, unsigned int curLine)
    : m_str(curData), m_line(curLine), m_next(NULL) { }
  ~StackNode() { if (m_next != NULL) delete m_next; }
  StackNode * add(const string& newStr, unsigned int newLine) {
    if (m_str.size() > 0) {
      // Assign new StackNode to m_next and return it
      return (m_next = new StackNode(newStr, newLine));
    }
    m_str = newStr;
    m_line = newLine;
    return this;
  }
  const string& getVal() const { return m_str; }
  unsigned int getLine() const { return m_line; }
  const StackNode* getNext() const { return m_next; }
  StackNode* getNext() { return m_next; }

 private:
  string m_str;
  unsigned int m_line;
  StackNode* m_next;
};

class ScriptState {
 public:
  explicit ScriptState(const string& name)
    : m_curLine(0), m_scriptName(name), m_parentState(ms_curScriptState),
      m_StackTop(new StackNode()) {
    ms_curScriptState = this;
  };
  ~ScriptState() {
    ms_curScriptState = m_parentState;
    if (m_StackTop)
      delete m_StackTop;
  };
  static unsigned int getCurLine() {
    return ms_curScriptState->m_curLine;
  };
  static string getLocation() {
    if (ms_curScriptState) return ms_curScriptState->desc();
    return EMPTYSTR;
  };
  static StackNode * getStackTop() { return ms_curScriptState->m_StackTop; }
  static void setCurLine(unsigned int curLine) {
    ms_curScriptState->m_curLine = curLine;
  };

 private:
  string desc() const {
    ostringstream toReturn;
    if (m_curLine > 0) {
      toReturn << " in " << m_scriptName << " at line number " << m_curLine;
    }
    if (m_parentState)
      toReturn << "\n\t" << m_parentState->desc();
    return toReturn.str();
  }
  unsigned int m_curLine;
  const string m_scriptName;
  ScriptState * const m_parentState;
  StackNode * m_StackTop;
  static ScriptState * ms_curScriptState;
};

class Output {
 public:
  static void setStreams(ostream &out, ostream &err) {
    StdOut = &out;
    StdErr = &err;
  }
  static ostream& Out() { return *StdOut; }
  static ostream& Err() { return *StdErr; }

 private:
  static ostream *StdOut;
  static ostream *StdErr;
  static string CurrentUser;
};

#endif  // OUTPUT_HPP
