/***************************************************************************
 * Parser.hpp
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
#if !defined(PARSER_HPP)
#  define PARSER_HPP

#  include <string>
#  if !defined(CALC_HPP)
#    include "Calc.hpp"
#  endif
#  if !defined(SYSTEMVAR_HPP)
#    include "SystemVar.hpp"
#  endif

class Parser {
 public:
  static void ParseScript(const std::string &filename);
  static void ParseLine(const std::string& lineIn, StackNode* &CurrentNode,
                        unsigned int CurrentLine);
  static StackNode *GetArgs(StackNode * StartNode, ArgListType &arg);

 private:
  static StackNode *CallAtFunction(StackNode * StartNode);
  static StackNode *CallCaretFunction(StackNode * StartNode, string&);
  static StackNode *DoConditional(StackNode * StartNode);
  static StackNode *DoLoop(StackNode * StartNode);
  static void FillStack(const std::string& filename);
  static StackNode *GetLoopVal(StackNode * StartNode, string& str);
  static StackNode *ParseStack(StackNode * StartNode);
  static void PrePostErrCheck(const bool ErrCondition, const std::string &pre,
                              const std::string &post);
  static StackNode *SkipStatement(StackNode * StartNode);
};

const std::string fnName = "^Fn(";
inline bool isFn(const std::string toChk) {
  return (toChk.compare(0, fnName.size(), fnName) == 0);
}

// For using Calc from elsewhere
double CalcFromStr(const std::string &toCalc);

DataList CalcFn(const std::string &calcStmt);

#endif
