/*******************************************************************************
 * ArgFuncts.cpp
 *
 *  Prototypes and definitions for C++ function argument processing,
 *
 *  Matt Harrison
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
 ******************************************************************************/
#if !defined(ARGFUNCTS_HPP)
#   include "ArgFuncts.hpp"
#endif

#include <string>

using std::string;

// findarg function
// Returns the index of a string in a list of strings or argc if the string is
// za not in the list
int findarg(const string str, const int &argc, const string argv[]) {
  for (int i = 0; i < argc; i++) {
    if (str == argv[i]) {
      return i;
    }
  }
  return argc;
}

void memcheck(const void * test, const string str) {
  if (test == 0) {
    cerr << str << endl;
    exit(EXIT_FAILURE);
  }
  return;
}

ostream& FlagArg::printHelp(ostream &os) const {
  int w = 10;
  string s = " : ";
  os << setw(w) << on_flag_ << s << setw(w);
  if (is_default_) {
    os << (default_val_ ? on_flag_ : off_flag_);
  } else {
    os << "{required}";
  }
  os << s << "{" << on_flag_ << "|" << off_flag_ << "} " << help_ << "\n";
  return os;
}

bool FlagArg::setValue(ArgListType &argv) {
  ArgListTypeIt onIt = find(argv.begin(), argv.end(), ArgType(on_flag_, false));
  ArgListTypeIt offIt = find(argv.begin(), argv.end(),
                             ArgType(off_flag_, false));
  bool foundOn = (onIt != argv.end());
  bool foundOff = (offIt != argv.end());
  // If both or neither are found, and there is no default behavior, bail
  if ((foundOn == foundOff) && !is_default_) return false;
  if (foundOff) {
    // Mark it as accounted for
    offIt->second = true;
    value_ = false;
  }
  if (foundOn) {
    // Mark it as accounted for
    onIt->second = true;
    value_ = true;  // On flag trumps off flag
  } else if (!foundOff) {
    value_ = default_val_;  // If neither are found, use default
  }
  return true;
}

void CommandLine::DisplayHelp(ostream & eout) const {
  unsigned int i = 0;
  eout << FuncName << " -help\n\n" << HelpString << "\n";

  for (i = 0; i < IntList.size(); i++) {
    IntList.at(i)->printHelp(eout);
  }

  for (i = 0; i < DblList.size(); i++) {
    DblList.at(i)->printHelp(eout);
  }

  for (i = 0; i < StrList.size(); i++) {
    StrList.at(i)->printHelp(eout);
  }

  for (i = 0; i < FlagList.size(); i++) {
    FlagList.at(i)->printHelp(eout);
  }

  for (i = 0; i < IntListList.size(); i++) {
    IntListList.at(i)->printHelp(eout);
  }

  for (i = 0; i < DblListList.size(); i++) {
    DblListList.at(i)->printHelp(eout);
  }

  for (i = 0; i < StrListList.size(); i++) {
    StrListList.at(i)->printHelp(eout);
  }

  eout << endl;
  return;
}

void CommandLine::IntSet(int number, ...) {
  if (number <= 0) return;

  va_list Ap;
  int oldSize = IntList.size();
  int newSize = oldSize + number;
  IntList.resize(newSize);
  va_start(Ap, number);
  for (int i = oldSize; i < newSize; i++) {
    IntList.at(i) = va_arg(Ap, TArg<int> *);
  }
  va_end(Ap);
  return;
}

void CommandLine::DblSet(int number, ...) {
  if (number <= 0) return;

  va_list Ap;
  int oldSize = DblList.size();
  int newSize = oldSize + number;
  DblList.resize(newSize);
  va_start(Ap, number);
  for (int i = oldSize; i < newSize; i++) {
    DblList.at(i) = va_arg(Ap, TArg<double> *);
  }
  va_end(Ap);
  return;
}

void CommandLine::StrSet(int number, ...) {
  if (number <= 0) return;

  va_list Ap;
  int oldSize = StrList.size();
  int newSize = oldSize + number;
  StrList.resize(newSize);
  va_start(Ap, number);
  for (int i = oldSize; i < newSize; i++) {
    StrList.at(i) = va_arg(Ap, TArg<string> *);
  }
  va_end(Ap);
  return;
}

void CommandLine::FlagSet(int number, ...) {
  if (number <= 0) return;

  va_list Ap;
  int oldSize = FlagList.size();
  int newSize = oldSize + number;
  FlagList.resize(newSize);
  va_start(Ap, number);
  for (int i = oldSize; i < newSize; i++) {
    FlagList.at(i) = va_arg(Ap, FlagArg *);
  }
  va_end(Ap);
  return;
}

void CommandLine::IntListSet(int number, ...) {
  if (number <= 0) return;

  va_list Ap;
  int oldSize = IntListList.size();
  int newSize = oldSize + number;
  IntListList.resize(newSize);
  va_start(Ap, number);
  for (int i = oldSize; i < newSize; i++) {
    IntListList.at(i) = va_arg(Ap, IntArgList *);
  }
  va_end(Ap);
}

void CommandLine::DblListSet(int number, ...) {
  if (number <= 0) return;

  va_list Ap;
  int oldSize = DblListList.size();
  int newSize = oldSize + number;
  DblListList.resize(newSize);
  va_start(Ap, number);
  for (int i = oldSize; i < newSize; i++) {
    DblListList.at(i) = va_arg(Ap, DblArgList *);
  }
  va_end(Ap);
}

void CommandLine::StrListSet(int number, ...) {
  if (number <= 0) return;

  va_list Ap;
  int oldSize = StrListList.size();
  int newSize = oldSize + number;
  StrListList.resize(newSize);
  va_start(Ap, number);
  for (int i = oldSize; i < newSize; i++) {
    StrListList.at(i) = va_arg(Ap, StrArgList *);
  }
  va_end(Ap);
  return;
}

void CommandLine::Process(ArgListType &argv, ostream & eout,
                          ostream & sout, const bool dolog) {
  string s = " : ";
  if (find(argv.begin(), argv.end(), ArgType("-help", false)) != argv.end()) {
    DisplayHelp(eout);
    exit(0);
  }
  if (dolog) {
    sout << "\n" << FuncName << " ";
  }

  unsigned int i;
  for (i = 0; i < IntList.size(); i++) {
    if (IntList.at(i)->setValue(argv)) {
      if (dolog) sout << *(IntList.at(i)) << " ";
    } else {
      eout << "Error in " << FuncName << " : Unable to find required flag "
           << IntList.at(i)->getFlag() << "\n\n";
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }

  for (i = 0; i < DblList.size(); i++) {
    if (DblList.at(i)->setValue(argv)) {
      if (dolog) sout << *(DblList.at(i)) << " ";
    } else {
      eout << "Error in " << FuncName << " : Unable to find required flag "
           << DblList.at(i)->getFlag() << "\n\n";
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }

  for (i = 0; i < StrList.size(); i++) {
    if (StrList.at(i)->setValue(argv)) {
      if (dolog) sout << *(StrList.at(i)) << " ";
    } else {
      eout << "Error in " << FuncName << " : Unable to find required flag "
           << StrList.at(i)->getFlag() << "\n\n";
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }

  for (i = 0; i < FlagList.size(); i++) {
    if (FlagList.at(i)->setValue(argv)) {
      if (dolog) sout << *(FlagList.at(i)) << " ";
    } else {
      eout << "Error in " << FuncName << " : Unable to process flag "
           << FlagList.at(i)->getFlag() << endl;
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }

  for (i = 0; i < IntListList.size(); i++) {
    if (IntListList.at(i)->setValue(argv)) {
      if (dolog) sout << *(IntListList.at(i));
    } else {
      eout << "Error in " << FuncName << s << IntListList.at(i)->getErr()
           << endl;
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }

  for (i = 0; i < DblListList.size(); i++) {
    if (DblListList.at(i)->setValue(argv)) {
      if (dolog) sout << *(DblListList.at(i));
    } else {
      eout << "Error in " << FuncName << s << DblListList.at(i)->getErr()
           << endl;
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }

  for (i = 0; i < StrListList.size(); i++) {
    if (StrListList.at(i)->setValue(argv)) {
      if (dolog) sout << *(StrListList.at(i));
    } else {
      eout << "Error in " << FuncName << s << StrListList.at(i)->getErr()
           << endl;
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }

  if (dolog)
    sout << "\n\n";

  for (ArgListTypeIt it = argv.begin(); it != argv.end(); it++) {
    // If not accounted for
    if (!it->second) {
      eout << "Error in " << FuncName << " : flag " << it->first
           << " not recognized." << endl;
      DisplayHelp(eout);
      exit(EXIT_FAILURE);
    }
  }
  return;
}
