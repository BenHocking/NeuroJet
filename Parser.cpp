/***************************************************************************
 * Parser.cpp
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
#  include "Parser.hpp"
#endif

#if !defined(OUTPUT_HPP)
#  include "Output.hpp"
#endif
#if !defined(PARALLEL_HPP)
#  include "Parallel.hpp"
#endif
#if !defined(SYMBOLS_HPP)
#  include "Symbols.hpp"
#endif
#if !defined(STRINGUTILS_HPP)
#  include "utils/StringUtils.hpp"
#endif

#include <istream>
#include <string>

using neurojet::stringutils::tokenize;

using std::ifstream;
using std::string;

void Parser::ParseScript(const string &filename) {
  ScriptState newScript = ScriptState(filename);
  FillStack(filename);
  ParseStack(ScriptState::getStackTop());
  return;
}

StackNode *Parser::CallAtFunction(StackNode * StartNode) {
  StackNode *CurrentNode = StartNode;
  
  const string& FunctionName = CurrentNode->getVal();
  
  ArgListType arg;
  
  // Get the AtFunction 
  AT_FUN AtFunPtr;
  if (SystemVar::GetVarType(FunctionName) != '@') {
    CALL_ERROR << "Could not find function @" << FunctionName << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  AtFunPtr = SystemVar::GetAtFun(FunctionName);
  PrePostErrCheck((CurrentNode->getNext() == NULL), "'@' Function", "'('");
  CurrentNode = CurrentNode->getNext();
  PrePostErrCheck((CurrentNode->getVal() != "("), "'@' Function", "'('");
  
  // Get the args 
  CurrentNode = GetArgs(CurrentNode->getNext(), arg);
  // Call the function 
  AtFunPtr(arg);
  
  return CurrentNode;
}

StackNode *Parser::CallCaretFunction(StackNode * StartNode, string& str) {
  StackNode *CurrentNode = StartNode;
  
  string FunctionName;
  /*
   * Check for function name, if none given, then default to GetVar 
   */
  if (CurrentNode->getVal() == "(") {
    FunctionName = "GetVar";
  } else {
    FunctionName = CurrentNode->getVal();
    PrePostErrCheck((CurrentNode->getNext() == NULL), "'^' function", "'('");
    CurrentNode = CurrentNode->getNext();
    PrePostErrCheck((CurrentNode->getVal() != "("), "'^' function", "'('");
  }
  
  /*
   * Get the Caret Function 
   */
  CARET_FUN CaretFunPtr;
  CaretFunPtr = SystemVar::GetCaretFun(FunctionName);
  if (CaretFunPtr == NULL) {
    CALL_ERROR << "Could not find function ^" << FunctionName << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  // Get the args
  ArgListType arg;
  CurrentNode = GetArgs(CurrentNode->getNext(), arg);
  
  // Call the function 
  str = CaretFunPtr(arg);
  
  return CurrentNode;
}

StackNode *Parser::DoConditional(StackNode * StartNode) {
  StackNode *CurrentNode = StartNode;
  
  string FunctionName = CurrentNode->getVal();;
  
  ArgListType arg;
  CurrentNode = CurrentNode->getNext();
  PrePostErrCheck(((CurrentNode ==  NULL) || (CurrentNode->getVal() != "(")),
                  "'?' function", "'('");
  
  // Get the args 
  StackNode *ArgNode = CurrentNode->getNext();
  CurrentNode = GetArgs(ArgNode, arg);
  
  // Verify beginning correctness
  if (FunctionName != "Read") {
    if ((CurrentNode == NULL) || (CurrentNode->getVal() != "{")) {
      CALL_ERROR << "Did not begin conditional with '{'" << ERR_WHERE;
      exit(EXIT_FAILURE);
    }
    if (CurrentNode->getNext() == NULL) {
      CALL_ERROR << "Nothing in conditional" << ERR_WHERE;
      exit(EXIT_FAILURE);
    }
    CurrentNode = CurrentNode->getNext();
  }
  if (arg.size() == 0) {
    CALL_ERROR << "Error in " << FunctionName << ": No args given" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  StackNode *StartLoopNode = CurrentNode;
  
  // Call the function
  if (FunctionName == "If") {
    if (fabs(from_string<float>(CalcCaret(arg))) > verySmallFloat) {
      CurrentNode = ParseStack(StartLoopNode);
      if ((CurrentNode == NULL) || (CurrentNode->getVal() != "}")) {
        CALL_ERROR << "Did not end ?If with '}'." << ERR_WHERE;
        exit(EXIT_FAILURE);
      }
      CurrentNode = CurrentNode->getNext();
    } else {
      CurrentNode = SkipStatement(StartLoopNode);
    }
  } else if (FunctionName == "While") {
    while (fabs(from_string<float>(CalcCaret(arg))) > verySmallFloat) {
      ParseStack(StartLoopNode);
      // Reprocess args
      GetArgs(ArgNode, arg);
    }
    // At top of Statement block, so must skip...
    CurrentNode = SkipStatement(StartLoopNode);
  } else if (FunctionName == "Read") {
    if (arg.size() != 1) {
      CALL_ERROR << "?Read takes exactly one argument." << ERR_WHERE;
      exit(EXIT_FAILURE);
    }
    ParseScript(arg.at(0).first);
  } else {
    CALL_ERROR << FunctionName << " not found." << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  return CurrentNode;
}

StackNode *Parser::DoLoop(StackNode * StartNode) {
  StackNode *CurrentNode = StartNode;
  PrePostErrCheck((StartNode->getVal() != "("), "'%'", "'('");
  
  ArgListType arg;
  // Get the Arguments 
  CurrentNode = GetArgs(CurrentNode->getNext(), arg);
  if ((arg.size() < 3) || (arg.size() > 4)) {
    CALL_ERROR << "Error in Loop: Must have 3 or 4 args:\n"
      "\tname start stop step, or\n"
      "\tname start stop.\n"
      "\t(step is assumed to be 1 if not given)\n" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  string VarName = arg.at(0).first;
  Iterator iter;
  iter.StartVal = from_string<int>(arg.at(1).first);
  iter.EndVal = from_string<int>(arg.at(2).first);
  iter.StepVal = (arg.size() == 3) ? 1 : from_string<int>(arg.at(3).first);
  iter.CurrentVal = iter.StartVal;
  if (iter.StepVal == 0) {
    CALL_ERROR << "The stepping variable is 0 (infinite loop)" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  // Check the ranges and idiot-proof
  iter.StepVal = (iter.StartVal < iter.EndVal) ? abs(iter.StepVal) : -abs(iter.StepVal);
  
  // Add the loop var and verify beginning correctness
  SystemVar::SetIterator(VarName, iter);
  if ((CurrentNode == NULL) || (CurrentNode->getVal() != "{")) {
    CALL_ERROR << "Did not begin loop with '{'." << ERR_WHERE;
    exit(EXIT_FAILURE);
  } else if (CurrentNode->getNext() == NULL) {
    CALL_ERROR << "Nothing to loop through." << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  // Do the Loop 
  CurrentNode = CurrentNode->getNext();
  StackNode *StartLoopNode = CurrentNode;
  int stepSign = sign(iter.StepVal);
  for (; stepSign * iter.CurrentVal <= stepSign * iter.EndVal; iter.CurrentVal += iter.StepVal) {
    SystemVar::SetIterator(VarName, iter);
    CurrentNode = ParseStack(StartLoopNode);
  }
  // Verify ending correctness
  if ((CurrentNode == NULL) || (CurrentNode->getVal() != "}")) {
    CALL_ERROR << "Did not end loop with '}'." << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  CurrentNode = CurrentNode->getNext();
  
  return CurrentNode;
}

void Parser::FillStack(const string& filename) {
  ifstream ScriptFile(filename.c_str());
  if (!ScriptFile) {
    IFROOTNODE
      CALL_ERROR << "Could not open script: " << filename << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  StackNode *CurrentNode;
  CurrentNode = ScriptState::getStackTop();
  unsigned int CurrentLine = 0;
  while (ScriptFile) {
    string lineIn;
    std::getline(ScriptFile, lineIn);
    ParseLine(lineIn, CurrentNode, ++CurrentLine);
  }
}

StackNode *Parser::GetArgs(StackNode * StartNode, ArgListType &arg) {
  StackNode * CurrentNode = StartNode;
  arg = ArgListType(0); // The empty vector
  if (CurrentNode->getVal() == ")") {
    return CurrentNode->getNext();
  }
  
  bool join = false;
  string JoinTemp = EMPTYSTR;
  string TempStr = EMPTYSTR;
  
  while (CurrentNode->getVal() != ")") {
    if (CurrentNode->getVal() == "^") {
      if (CurrentNode->getNext() == NULL) {
        CALL_ERROR << "No name given for CaretFunction" << ERR_WHERE;
        exit(EXIT_FAILURE);
      } else {
        CurrentNode = CallCaretFunction(CurrentNode->getNext(), TempStr);
      }
    } else if (CurrentNode->getVal() == "$") {
      if (CurrentNode->getNext() == NULL) {
        CALL_ERROR << "No variable given for loop reference" << ERR_WHERE;
        exit(EXIT_FAILURE);
      } else {
        CurrentNode = GetLoopVal(CurrentNode->getNext(), TempStr);
      }
    } else {
      TempStr = CurrentNode->getVal();
      CurrentNode = CurrentNode->getNext();
    }
    if (CurrentNode == NULL) {
      CALL_ERROR << "Parse error on last line of file: Missing ')'" << ERR_WHERE;
      exit(EXIT_FAILURE);
    }
    // Check for joining of args 
    if (join) {
      if (TempStr != ":") {
        JoinTemp += TempStr;
      } else {
        TempStr = EMPTYSTR; // Don't actually attach the ":"
        continue;
      }
    } else {
      JoinTemp = TempStr;
    }
    TempStr = EMPTYSTR;
    join = (CurrentNode->getVal() == ":");
    // If the next node is not a join symbol, then add it to list 
    if (!join) {
      arg.push_back(ArgType(JoinTemp, false));
    }
  }
  
  // If the join is true, then trying to join to nothing 
  if (join) {
    CALL_ERROR << "Trying to join to nothing." << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  return CurrentNode->getNext();
}

StackNode *Parser::GetLoopVal(StackNode * StartNode, string& str) {
  StackNode *CurrentNode = StartNode;
  PrePostErrCheck(((CurrentNode ==  NULL) || (CurrentNode->getVal() != "(")),
                  "'$'", "'('");
  if (CurrentNode->getNext() == NULL) {
    CALL_ERROR << "Must put a loop variable name" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  CurrentNode = CurrentNode->getNext();
  
  string VarName = CurrentNode->getVal();
  
  // Get the val 
  if (SystemVar::GetVarType(VarName) != 'I') {
    CALL_ERROR << "Did not find loop var " << VarName << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  str = to_string(SystemVar::GetIterator(VarName).CurrentVal);
  
  CurrentNode = CurrentNode->getNext();
  if ((CurrentNode == NULL) || (CurrentNode->getVal() != ")")) {
    CALL_ERROR << "Must close loop var with ')'" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  return CurrentNode->getNext();
}

void Parser::ParseLine(const string& lineIn, StackNode* &CurrentNode,
                       unsigned int CurrentLine) {
  unsigned int curChar = 0;
  while (curChar < lineIn.size()) {
    char ch = lineIn[curChar++]; // Read curChar and move to next
    // If it is a Comment, stop reading
    if (ch == CommentSymbol) break;
    
    // If it is a Quote symbol 
    if (ch == QuoteSymbol) {
      string TempStr = EMPTYSTR;
      ch = lineIn[curChar++];
      
      while (curChar < lineIn.size() && (ch != QuoteSymbol)) {
        if (ch == '\\') {
          char ch2 = lineIn[curChar++];
          string escSymbols = "n\nt\tv\vb\br\rf\fa\a";
          if ((ch2 == QuoteSymbol) || (ch2 == '\\')) {
            ch = ch2;
          } else {
            bool foundChar = false;
            for (unsigned int i=0; i < escSymbols.size(); i+=2) {
              if (ch2 == escSymbols[i]) {
                ch = escSymbols[i+1];
                foundChar = true;
                break;
              }
            }
            if (!foundChar) {
              CALL_ERROR << "Unrecognized escape symbol " << ch << ch2
                         << ERR_WHERE;
              exit(EXIT_FAILURE);
            }
          }
        }
        TempStr += ch;
        ch = lineIn[curChar++];
      }
      if (ch != QuoteSymbol) {
        CALL_ERROR << "End of line reached before end of quote" << ERR_WHERE;
        exit(EXIT_FAILURE);
      }
      CurrentNode = CurrentNode->add(TempStr, CurrentLine);
    } else if (GrammarSymbol(ch)) {
      // If another symbol 
      string TempStr;
      TempStr = ch;
      CurrentNode = CurrentNode->add(TempStr, CurrentLine);
    } else if (isalnum(ch) || ispunct(ch)) {
      // If an alpha numeric symbol 
      string TempStr = EMPTYSTR;
      while (curChar < lineIn.size() && (isalnum(ch) || ispunct(ch)) && !GrammarSymbol(ch)) {
        TempStr += ch;
        ch = lineIn[curChar++];
      }
      if ((isalnum(ch) || ispunct(ch)) && !GrammarSymbol(ch)) {
        TempStr += ch;
      } else {
        // Make sure ch is used on next round!
        --curChar;
      }
      CurrentNode = CurrentNode->add(TempStr, CurrentLine);
    }
  }
}

StackNode *Parser::ParseStack(StackNode * StartNode) {
  if (StartNode == NULL) return StartNode;
  StackNode *CurrentNode = StartNode;
  
  ScriptState::setCurLine(CurrentNode->getLine());
  
  // If AtFunction 
  if (CurrentNode->getVal() == "@") {
    if (CurrentNode->getNext() == NULL) {
      CALL_ERROR << "No name given for AtFunction" << ERR_WHERE;
      exit(EXIT_FAILURE);
    } else {
      CurrentNode = CallAtFunction(CurrentNode->getNext());
    }
  }
  
  // If CondFunction 
  else if (CurrentNode->getVal() == "?") {
    if (CurrentNode->getNext() == NULL) {
      CALL_ERROR << "No name given for Conditional Function" << ERR_WHERE;
      exit(EXIT_FAILURE);
    } else {
      CurrentNode = DoConditional(CurrentNode->getNext());
    }
  }
  
  // If Caret Symbol 
  else if (CurrentNode->getVal() == "^") {
    CALL_ERROR << "Can't call Caret Function from outside function" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  // If loop symbol 
  else if (CurrentNode->getVal() == "%") {
    if (CurrentNode->getNext() == NULL) {
      CALL_ERROR << "No Parameters for loop" << ERR_WHERE;
      exit(EXIT_FAILURE);
    } else {
      CurrentNode = DoLoop(CurrentNode->getNext());
    }
  }
  
  // If LoopIndex Symbol 
  else if (CurrentNode->getVal() == "$") {
    CALL_ERROR << "Can't call Loop Index from outside function" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  
  // If end of loop 
  else if (CurrentNode->getVal() == "}") {
    return CurrentNode;
  }
  
  else {          /* Ignore everything else */
    CurrentNode = CurrentNode->getNext();
  }
  
  CurrentNode = ParseStack(CurrentNode);
  return CurrentNode;
}

void Parser::PrePostErrCheck(const bool ErrCondition, const std::string &pre,
                             const std::string &post) {
  if (ErrCondition) {
    CALL_ERROR << "Must put " << post << " after " << pre << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
}

StackNode *Parser::SkipStatement(StackNode * StartNode) {
  StackNode *CurrentNode = StartNode;
  int NumIn = 0;
  while (CurrentNode != NULL) {
    if (CurrentNode->getVal() == "}") {
      if (NumIn == 0) {
        break;
      } else {
        NumIn--;
      }
    }
    if (CurrentNode->getVal() == "{") {
      NumIn++;
    }
    CurrentNode = CurrentNode->getNext();
  }
  if (CurrentNode == NULL) {
    CALL_ERROR << "Parse error at EOF: Did not end loop with '}'.\n" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
  CurrentNode = CurrentNode->getNext();
  
  return CurrentNode;
}

double CalcFromStr(const string &toCalc) {
  StackNode *CurrentNode = new StackNode();
  StackNode *ThisTopNode = CurrentNode;
  Parser::ParseLine(toCalc + ")", CurrentNode, ScriptState::getCurLine());
  ArgListType arg;
  CurrentNode = Parser::GetArgs(ThisTopNode, arg);
  return from_string<double>(CalcCaret(arg));
}

DataList CalcFn(const string &calcStmt) {
  DataList toReturn;
  const unsigned int argStart = fnName.size();
  const unsigned int argSize = calcStmt.size()-argStart-1;
  const string s(calcStmt, argStart, argSize);
  vector<string> filterArgs = tokenize(s, ',', "\"\"[]()");
  unsigned int numArgs = filterArgs.size();
  if (numArgs != 3)  {
    CALL_ERROR << "Incorrect number of arguments (" << numArgs << ") to " + fnName + ")" << ERR_WHERE;
  }
  const float minT = CalcFromStr(filterArgs[0]);
  const float maxT = CalcFromStr(filterArgs[1]);
  const float deltaT = SystemVar::GetFloatVar("deltaT");
  if (deltaT < verySmallFloat) {
    CALL_ERROR << "deltaT must be defined before using " + fnName + ")" << ERR_WHERE;
  }
  // Using + deltaT/2 is a rounding mechanism. Just using + verySmallFloat was
  // causing it to miss even when deltaT divided maxT evenly, and even when
  // doubles were used instead of floats.
  for (double t=minT; t < (maxT + deltaT/2); t += deltaT) {
    SystemVar::SetFloatVar("tFn", t);
    float newVal = CalcFromStr(filterArgs[2]);
    toReturn.push_back(newVal);
  }
  return toReturn;
}
