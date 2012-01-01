/***************************************************************************
 * Symbols.hpp
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
#if !defined(SYMBOLS_HPP)
#define SYMBOLS_HPP

#  include <string>

#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif

const int NumGrammarSymbols = 13;
const char StartNodeSymbol = '{';
const char EndNodeSymbol = '}';
const char CommentSymbol = '#';
const char AtSymbol = '@';
const char CaretSymbol = '^';
const char LoopSymbol = '%';
const char LoopIndexSymbol = '$';
const char MacroDefSymbol = ']';
const char StartParameterSymbol = '(';
const char EndParameterSymbol = ')';
const char QuoteSymbol = '\"';
const char JoinSymbol = ':';
const char ConditionalSymbol = '?';

const std::string GrammarSymbolList =  // NOLINT(runtime/string)
  " {}#@^%$]()\":?";

inline int GrammarSymbol(const char &c) {
  return (GrammarSymbolList.find(c) == std::string::npos) ? 0
    : GrammarSymbolList.find(c);
}

const std::string SymbolHelp[NumGrammarSymbols + 1] = {
  "Invalid Symbol.",
  "StartNodeSymbol... Used for enclosing commands after a loop.",
  "EndNodeSymbol... Used for enclosing commands after a loop.",
  "CommentSymbol... Used for beginning a comment line that\n"
  "\t   is ignored by the program.",
  "AtSymbol... Used for indicating that the next function\n"
  "\t   is a void function. ",
  "CaretSymbol... Used for indicating that the next function\n"
  "\t   returns a string. Alone it stands for ^GetVar.",
  "LoopSymbol... Used for beginning a loop function.\n"
  "\t   The only loop function is( .... ).",
  "LoopIndexSymbol... Used for accessing the value of a loop\n"
  "\t   iterator.",
  "MacroDefSymbol... Under Construction... ",
  "StartParameterSymbol... Used for enclosing the parameters\n"
  "\t   of the previous function.",
  "EndParameterSymbol... Used for enclosing the parametes of\n"
  "\t   the previous function.",
  "QuoteSymbol... Used around strings to keep the program\n"
  "\t   from parsing them.",
  "JoinSymbol... Used for concatenating strings into one.",
  "ConditionalSymbol... Used before a conditional function:\n"
  "\t   If or While. \n\n"
  "Conditional If -help\n\n"
  "?If(...) { ... } evaluates a numerical expression and executes\n"
  "the subsequent braced statement if true.\n"
  "The syntax is ?If( expression ) { .... }\n"
  "Expression can be number or a ^Calc expression.\n\n\n"
  "Conditional While -help\n\n"
  "?While(...) { ... } evaluates a numerical expression and executes\n"
  "the subsequent braced statement while it remains true.\n"
  "The syntax is ?While( expression ) { .... }\n"
  "Expression can be number or a ^Calc expression.\n"
};

#endif
