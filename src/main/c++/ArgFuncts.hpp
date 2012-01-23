/***************************************************************************
 * ArgFuncts.hpp
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
 ****************************************************************************/
//
// High Level Command Line Processing
//
// class IntArg(const string flag, const string help, int default)
// class IntArg(const string flag, const string help)
// class DblArg(const string flag, const string help, double default)
// class DblArg(const string flag, const string help)
// class StrArg(const string flag, const string help, string default)
// class StrArg(const string flag, const string help)
// class FlagArg(const string onflag, const string offflag, const string help,
//               int default)
// class FlagArg(const string onflag, const string offflag, const string help)
//
// class CommandLine(string FunctionName)
// CommandLine.IntSet(int numints, IntArg * arg1, ...)
// CommandLine.DblSet(int numdbls, DblArg * arg1, ...)
// CommandLine.StrSet(int numstrs, StrArg * arg1, ...)
// CommandLine.FlagSet(int numflags, FlagArg * arg1, ...)
// CommandLine.HelpSet(string helpstr);
// CommandLine.Process(int argc, const string argv[], ostream& errorout)
// CommandLine.Process(int argc, const string argv[], ostream& errorout,
//                     ostream& callout)
// CommandLine.DisplayHelp(ostream &helpout)
//
// The IntArg, DblArg, and StrArg classes allow you to define integers,
// doubles, and strings for commandline processing. The first parameter
// is the flag that is searched for in the command line. The second is
// the help message that should be printed when -help is invoked. The
// third is the value that the variable should default to. If no default
// is given then the program will exit with an error if the indicated flag
// is not found in the command line.
// The FlagStr class allows you to define flags for commandline processing.
// The first parameter is the on flag, the second is the off flag, and the
// third is the help. If the onflag is found in the commandline then the
// variable takes a value of 1. If the offflag is found then it takes
// a value of zero. If neither are found then it becomes the default{0,1}.
// If both are found then it becomes the opposite of the default. If no
// default is given then exactly one of the flags(on or off) must be
// present in the commandline or the program exits with an error.
//
// The CommandLine class processing the command lines. After all of the
// Arg types are declared, then they must be passed (as pointers) to the
// CommandLine variable using the Set functions.
//
// Finally, the Process function is given argc and argv and the command line
// is processed. If -help is encountered, then the help commands are
// printed and the program exits. Any unrecognized arguments are reported.
// The complete function call with all defaults inserted is echoed to the
// screen. The values of the variables can be accessed and manipulated
// with the() operator.
//
//
///////////////////////////////////////////////////////////////////////////////
//
// Lower Level CommandLine Processing Functions
//
// int findarg(string str, int argc, string argv[])
//
// findarg returns the index for which argv[index] = str
// if no such index exists, then it returns argc
//
// examples:
// if (findarg("-help",argc,argv) == argc) cerr << "Sorry, No Help." << endl;
// int numfiles = findarg("-nf",argc,argv);
//
///////////////////////////////////////////////////////////////////////////////
//
// Numerical string processing commands and basic string processing commands
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(ARGFUNCTS_HPP)
#define ARGFUNCTS_HPP

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdarg>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#if !defined(STRINGUTILS_HPP)
#  include "utils/StringUtils.hpp"
#endif

#if !defined(MATRIXMEANHELPER_HPP)
#   include "MatrixMeanHelper.hpp"
#endif

#if !defined(MATRIXMOMENTHELPER_HPP)
#   include "MatrixMomentHelper.hpp"
#endif

#if !defined(MATRIXSSHELPER_HPP)
#   include "MatrixSSHelper.hpp"
#endif

#if !defined(MATRIXSUMHELPER_HPP)
#   include "MatrixSumHelper.hpp"
#endif

#if !defined(DATATYPES_HPP)
#   include "DataTypes.hpp"
#endif

#if !defined(OUTPUT_HPP)
#   include "Output.hpp"
#endif

using std::cerr;
using std::cout;
using std::endl;
using std::find;
using std::list;
using std::map;
using std::max;
using std::min;
using std::ostream;
using std::pair;
using std::ptr_fun;
using std::setw;
using std::size_t;
using std::string;
using std::transform;
using std::vector;

using neurojet::stringutils::to_string;
using neurojet::stringutils::from_string;


////////////////////////////////////////////////////////////////////////////////
// Some matrix/vector functions (see also setMatrixRange, which depends on
// IntArg)
////////////////////////////////////////////////////////////////////////////////

template <typename T>
T vectorSum(const vector<T> &toSum) {
  return accumulate(toSum.begin(), toSum.end(), T(0));
}

// sums over columns (into rows)
template <typename T>
vector<T> matrixSum(const vector<vector<T> > &toSum) {
  vector<T> toReturn;
  transform(toSum.begin(), toSum.end(), back_inserter(toReturn), vectorSum<T>);
  return toReturn;
}

template <typename T>
double matrixMean(const vector<vector<T> > &toAvg) {
  return for_each(toAvg.begin(), toAvg.end(), MatrixMeanHelper<T>()).result();
}

template <typename T>
double matrixMoment(const vector<vector<T> > &Matrix, const float avg,
                    const int moment) {
  return for_each(Matrix.begin(), Matrix.end(),
                  MatrixMomentHelper<T>(avg, moment)).result();
}

template <typename T>
double matrixAvgSS(const vector<vector<T> > &Matrix) {
  return for_each(Matrix.begin(), Matrix.end(), MatrixSSHelper<T>()).result();
}

template <typename T>
size_t matrixSize(const vector<vector<T> > &Matrix) {
  const size_t size = Matrix.size() > 0 ? Matrix.size() * Matrix[0].size() : 0;
  return size;
}

template <typename T>
double matrixVar(const vector<vector<T> > &Matrix) {
  const double avg = matrixMean(Matrix);
  const size_t size = matrixSize(Matrix);
  // Var = sum((x-mean(x))^2) / (n - 1)
  // sum((x-mean(x))^2) = sum(x^2 - 2x*mean(x) + mean(x)^2)
  //   = sum(x^2) - 2n*mean(x)^2 + n*mean(x)^2 = sum(x^2) + n * mean(x)^2
  // matrixAvgSS returns sum(x^2) / n
  // so n * (matrixAvgSS - mean(x)^2) = sum(x^2) + n * mean(x)^2
  // Dividing that by (n - 1) should give variance
  return (matrixAvgSS(Matrix) - avg * avg) * size / (size  - 1);
}

template <typename T>
double matrixSkew(const vector<vector<T> > &Matrix) {
  const double avg = matrixMean(Matrix);
  const double var = matrixVar(Matrix);
  const double moment3 = matrixMoment(Matrix, avg, 3);
  return moment3 / pow(var, 1.5);
}

template <typename T>
double matrixKurt(const vector<vector<T> > &Matrix) {
  const double avg = matrixMean(Matrix);
  const double var = matrixVar(Matrix);
  const double moment4 = matrixMoment(Matrix, avg, 4);
  // The modern definition of kurtosis includes the "-3" term
  // Thus, a normal distribution has zero kurtosis
  return moment4 / (var*var) - 3;
}

template<class T>
inline unsigned int findMaxSize(const T &dataIn,
                                const unsigned int PriorMax = 0) {
  typename T::const_iterator it;
  unsigned int MaxSize = PriorMax;
  for (it = dataIn.begin(); it != dataIn.end(); it++) {
    updateMax(MaxSize, static_cast<unsigned int>(it->size()));
  }
  return MaxSize;
}

template<>  // Template specialization
inline unsigned int findMaxSize(const UIPtnSequence &dataIn,
                                const unsigned int PriorMax) {
  unsigned int MaxSize = PriorMax;
  for (UIPtnSequenceCIt it = dataIn.begin(); it != dataIn.end(); it++) {
    updateMax(MaxSize, it->back()+1);  // assumes ptns in dataIn are sorted
  }
  return MaxSize;
}

// works on vector<vector<foo> >, list<list<foo> >, list<vector<foo> >, etc.
// but slower than necessary
template <typename T>
T transposeMatrixSlow(const T &toTranspose) {
  unsigned int oldRowSize = toTranspose.size();
  unsigned int oldColSize = findMaxSize(toTranspose);
  typename T::const_iterator itSrcRow;
  typename T::value_type::const_iterator itSrcCol;
  typename T::iterator itDstRow;
  typename T::value_type::iterator itDstCol;
  typename T::value_type emptySubType(oldRowSize);
  T toReturn(oldColSize, emptySubType);
  itDstRow = toReturn.begin();
  unsigned int dstColCnt = 0;
  for (itSrcRow = toTranspose.begin(); itSrcRow != toTranspose.end();
       ++itSrcRow, ++dstColCnt) {
    itDstRow = toReturn.begin();
    for (itSrcCol = itSrcRow->begin(); itSrcCol != itSrcRow->end();
         ++itSrcCol, ++itDstRow) {
      itDstCol = itDstRow->begin();
      for (unsigned int inc = 0; inc < dstColCnt; ++inc, ++itDstCol) {}
      *itDstCol = *itSrcCol;
    }
  }
  return toReturn;
}

template <typename T>
vector<vector<T> > transposeMatrix(const vector<vector<T> > &toTranspose) {
  unsigned int oldRowSize = toTranspose.size();
  unsigned int oldColSize = findMaxSize(toTranspose);
  typename vector<vector<T> >::const_iterator itSrcRow;
  typename vector<T>::const_iterator itSrcCol;
  typename vector<vector<T> >::iterator itDstRow;
  vector<vector<T> > toReturn(oldColSize, vector<T>(oldRowSize,
                                                    static_cast<T>(0)));
  itDstRow = toReturn.begin();
  unsigned int dstColCnt = 0;
  for (itSrcRow = toTranspose.begin(); itSrcRow != toTranspose.end();
       ++itSrcRow, ++dstColCnt) {
    itDstRow = toReturn.begin();
    for (itSrcCol = itSrcRow->begin(); itSrcCol != itSrcRow->end();
         ++itSrcCol, ++itDstRow) {
      (*itDstRow)[dstColCnt] = *itSrcCol;
    }
  }
  return toReturn;
}

// first & last are 1-based, VecIn & toReturn are 0-based
template<typename T>
inline vector<T> SubVector(const vector<T> &VecIn, unsigned int first,
                           unsigned int last) {
  unsigned int useLast = last == 0 ? VecIn.size()
    : min(last, static_cast<unsigned int>(VecIn.size()));
  vector<T> toReturn(useLast - first + 1, static_cast<T>(0));
  copy(VecIn.begin() + first - 1, VecIn.begin() + useLast, toReturn.begin());
  return toReturn;
}

// first & last are 1-based, mtxIn & toReturn are 0-based
template<typename T>
inline vector<vector<T> > SubMatrix(const vector<vector<T> > &mtxIn,
                                    unsigned int firstT, unsigned int lastT,
                                    unsigned int firstN, unsigned int lastN) {
  unsigned int numOldRows = mtxIn.at(0).size();
  unsigned int numOldCols = mtxIn.size();
  unsigned int useLastRow = lastT == 0 ? numOldRows : min(lastT, numOldRows);
  unsigned int useLastCol = lastN == 0 ? numOldCols : min(lastN, numOldCols);
  unsigned int numRows = useLastRow - firstT + 1;
  unsigned int numCols = useLastCol - firstN + 1;
  vector<vector<T> > toReturn(numRows, vector<T>(numCols, static_cast<T>(0)));
  typename vector<vector<T> >::const_iterator MCIt = mtxIn.begin() + firstT - 1;
  typename vector<vector<T> >::iterator MIt = toReturn.begin();
  for (unsigned int i = firstT-1; i < useLastRow; ++i, ++MCIt, ++MIt) {
    *MIt = SubVector(*MCIt, firstN, useLastCol);
  }
  return toReturn;
}

////////////////////////////////////////////////////////////////////////////////
// Some integer functions
////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline int ifloor(const T data_in) { return static_cast<int>(data_in); }

template<typename T>
inline int iround(const T data_in) { return static_cast<int>(data_in + 0.5); }

template<typename T>
inline int iceil(const T data_in) { return static_cast<int>(ceil(data_in)); }

////////////////////////////////////////////////////////////////////////////////
// Some data deallocation functions
////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline void delPtr(T &ptr) {
  if (ptr) {
    delete ptr;
    ptr = NULL;
  }
}

template<typename T>
inline void delArray(T &ptr) {
  if (ptr) {
    delete[] ptr;
    ptr = NULL;
  }
}

template<typename T>
inline void delMatrix(T &ptr, int height) {
  if (ptr) {
    for (int i = 0; i < height; i++) {
      delArray(ptr[i]);
    }
    delete[] ptr;
    ptr = NULL;
  }
}

template<typename T>
inline void delTensor(T &ptr, int otherDim, int height) {
  if (ptr) {
    for (int i = 0; i < otherDim; i++) {
      delMatrix(ptr[i], height);
    }
    delete[] ptr;
    ptr = NULL;
  }
}

inline void memcheck(const void * test, const string str = "Out of memory");

////////////////////////////////////////////////////////////////////////////////
// Some common TArg routines
////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline bool setTValue(ArgListType &argv, const string& Flag, T &Value,
                      const bool IsDeault, const T Deault) {
  // Find an argument that matches the flag, but that hasn't already been found
  ArgListTypeIt thing1 = find(argv.begin(), argv.end(), ArgType(Flag, false));
  if (thing1 != argv.end()) {
    // Check out its corresponding value
    ArgListTypeIt thing2 = thing1 + 1;
    // If we're not at the end and not already marked as being found
    if ((thing2 != argv.end()) && (!thing2->second)) {
      // Mark it and its value as being accounted for
      thing1->second = true;
      thing2->second = true;
      Value = from_string<T>(thing2->first);
      return true;
    }
  }
  if (!IsDeault) return false;
  Value = Deault;
  return true;
}

template<typename T>
string setListValue(ArgListType &argv, const string& Flag,
                    vector<T> &Value, const bool IsDeault,
                    const vector<T> Deault) {
  // Find an argument that matches the flag, but that hasn't already been found
  ArgListTypeIt flagpos = find(argv.begin(), argv.end(), ArgType(Flag, false));
  ArgListTypeIt assignpos = flagpos;
  // Move to the value corresponding to the flag
  if (assignpos != argv.end()) assignpos++;
  if (assignpos == argv.end() && !IsDeault) {
    return "Unable to find required flag " + Flag;
  }
  if (assignpos == argv.end()) {
    // Mark the flag accounted for if it was given with no arguments and
    // default is allowed
    if (flagpos != argv.end()) flagpos->second = true;
    Value = Deault;
  } else {
    int numvals = from_string<int>(assignpos->first);
    if (numvals < 0) {
      return "Bad number of arguments following " + Flag;
    }
    Value = vector<T>(numvals);
    // Mark the flag accounted for
    flagpos->second = true;
    // Mark the # accounted for
    assignpos->second = true;
    assignpos++;  // Move to the first item in the list (after the #)
    for (int j = 0; j < numvals; j++, assignpos++) {
      // If the argument is already accounted for
      if (assignpos->second) {
        return "Unable to find " + to_string(numvals) + " arguments after "
          + Flag;
      }
      // Mark the value accounted for
      assignpos->second = true;
#if defined(CHECK_BOUNDS)
      Value.at(j) = from_string<T>(assignpos->first);
#else
      Value[j] = from_string<T>(assignpos->first);
#endif
    }
  }
  return EMPTYSTR;  // No errMsg
}

template<typename T>
inline void printTHelp(ostream &os, const string &Flag, const string &Help,
                       const bool IsDeault, const T &Deault) {
  int w = 10;
  string s = " : ";
  os << std::setw(w) << Flag << s << std::setw(w)
     << (IsDeault ? to_string<T>(Deault) : "{required}")
     << s << Help << "\n";
}

template<typename T>
void printListHelp(ostream &os, const string &Flag, const string &Help,
                   const bool IsDeault, const vector<T> &Deault) {
  int w = 10;
  string s = " : ";
  os << std::setw(w - 3) << Flag << " # " << s << std::setw(w);
  if (IsDeault) {
    string nada = "{0 items}";
    os << (Deault.size() ? to_string<T>(Deault.at(0)) : nada) << s;
    for (unsigned int i = 1; i < Deault.size(); i++)
      os << "\n" << std::setw(w) << " " << s << std::setw(w) << Deault[i] << s;
  } else {
    os << "{required}" << s;
  }
  os << Help << "\n";
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
class TArg {
 public:
  TArg(const string& flag, const string& help, const T defaultVal)
    : value_(defaultVal), flag_(flag), help_(help), default_val_(defaultVal),
      is_default_(true) { }
  TArg(const string& flag, const string& help)
  : value_(0), flag_(flag), help_(help), default_val_(0), is_default_(false) { }
  virtual ~TArg() { }
  const string& getFlag() const { return flag_; }
  const T& getValue() const { return value_; }
  virtual ostream& printHelp(ostream& os) const {
    int w = 10;
    const string s = " : ";
    os << std::setw(w) << flag_;
    os << s;
    os << std::setw(w) << (is_default_ ? to_string<T>(default_val_)
                           : "{required}");
    os << s << help_ << std::endl;
    return os << std::setw(w) << flag_ << s << std::setw(w)
              << (is_default_ ? to_string<T>(default_val_) : "{required}")
              << s << help_ << "\n";
  }
  virtual bool setValue(const T& newValue) {
    value_ = newValue;
    return true;
  }
  void swapValue(TArg& otherArg) { std::swap(value_, otherArg.value_); }
  virtual bool setValue(ArgListType &argv) {
    // Find an argument that matches the flag, but not already found
    ArgListTypeIt thing1 = find(argv.begin(), argv.end(),
                                ArgType(flag_, false));
    if (thing1 != argv.end()) {
      // Check out its corresponding value
      ArgListTypeIt thing2 = thing1 + 1;
      // If we're not at the end and not already marked as being found
      if ((thing2 != argv.end()) && (!thing2->second)) {
        // Mark it and its value as being accounted for
        thing1->second = true;
        thing2->second = true;
        value_ = from_string<T>(thing2->first);
        return true;
      }
    }
    if (!is_default_) return false;
    value_ = default_val_;
    return true;
  }

 protected:
  T value_;
  string flag_;
  string help_;
  T default_val_;
  bool is_default_;
};

// Template specialization for strings
template <>
inline TArg<string>::TArg(const string& flag, const string& help)
  : value_(EMPTYSTR), flag_(flag), help_(help), default_val_(EMPTYSTR),
    is_default_(false) { }

template <typename T>
ostream& operator<<(ostream &os, const TArg<T> &x) {
  return (os << x.getFlag() << " " << x.getValue());
}

class IntArgList: public TArg<vector<int> > {
 public:
  IntArgList(const string& flag, const string& help)
    : TArg<vector<int> >(flag, help), err_msg_(EMPTYSTR) { }
  inline const string& getErr() const { return err_msg_; }
  inline int getValue(const int i) const { return value_.at(i); }
  ostream& printHelp(ostream &os) const {
    printListHelp(os, flag_, help_, is_default_, default_val_);
    return os;
  }
  bool setValue(ArgListType &argv) {
    err_msg_ = setListValue(argv, flag_, value_, is_default_, default_val_);
    return err_msg_.size() == 0;
  }
  inline unsigned int size() const { return value_.size(); }
  inline int& operator[] (int index) { return value_.at(index); }
  inline const int& operator[] (int index) const { return value_.at(index); }

 private:
  string err_msg_;
};

inline ostream& operator << (ostream &os, const IntArgList &x) {
  int numvals = x.size();
  os << x.getFlag() << " " << numvals << " ";
  for (int i = 0; i < numvals; i++)
    os << x.getValue(i) << " ";
  return os;
}

class DblArgList: public TArg<vector<double> > {
 public:
  DblArgList(const string& flag, const string& help)
    : TArg<vector<double> >(flag, help), err_msg_(EMPTYSTR) { }
  inline const string& getErr() const { return err_msg_; }
  inline double getValue(const int i) const { return value_.at(i); }
  ostream& printHelp(ostream &os) const {
    printListHelp(os, flag_, help_, is_default_, default_val_);
    return os;
  }
  bool setValue(ArgListType &argv) {
    err_msg_ = setListValue(argv, flag_, value_, is_default_, default_val_);
    return err_msg_.size() == 0;
  }
  inline unsigned int size() const { return value_.size(); }
  inline double &operator[] (int index) { return value_[index]; }
  inline const double &operator[] (int index) const { return value_[index]; }

 protected:
  string err_msg_;
};

inline ostream& operator << (ostream &os, const DblArgList &x) {
  int numvals = x.size();
  os << x.getFlag() << " " << numvals << " ";
  for (int i = 0; i < numvals; i++)
    os << x.getValue(i) << " ";
  return os;
}

class StrArgList: public TArg<vector<string> > {
 public:
  StrArgList(const string& flag, const string& help,
             const bool isOptional = false)
    : TArg<vector<string> >(flag, help), is_optional_(isOptional),
      err_msg_(EMPTYSTR) { }
  inline const string& getErr() const { return err_msg_; }
  inline const string& getValue(const int i) const { return value_.at(i); }
  ostream& printHelp(ostream &os) const {
    printListHelp(os, flag_, help_, is_optional_, default_val_);
    return os;
  }
  bool setValue(ArgListType &argv) {
    err_msg_ = setListValue(argv, flag_, value_, is_optional_, default_val_);
    return err_msg_.size() == 0;
  }
  inline unsigned int size() const {return value_.size(); }
  inline string& operator[] (int index) { return value_.at(index); }
  inline const string& operator[] (int index) const { return value_.at(index); }

 private:
  bool is_optional_;
  string err_msg_;
};

inline ostream& operator << (ostream &os, const StrArgList &x) {
  int numvals = x.size();
  os << x.getFlag() << " " << numvals << " ";
  for (int i = 0; i < numvals; i++)
    os << x.getValue(i) << " ";
  return os;
}

class FlagArg: public TArg<bool> {
 public:
  FlagArg(const string& onFlag, const string& offFlag, const string& help,
          bool defaultVal)
    : TArg<bool>(onFlag, help, defaultVal), on_flag_(onFlag),
      off_flag_(offFlag) { }
  FlagArg(const string& onFlag, const string& offFlag, const string& help)
    : TArg<bool>(onFlag, help), on_flag_(onFlag), off_flag_(offFlag) { }
  ostream& printHelp(ostream &os) const;
  bool setValue(bool newValue) {
    value_ = newValue;
    return true;
  }
  bool setValue(ArgListType &argv);

 protected:
  string on_flag_;
  string off_flag_;
};

class CommandLine {
 public:
  explicit CommandLine(const string funcname): FuncName(funcname),
                                               HelpString(EMPTYSTR) { }
  ~CommandLine() {}

  void IntSet(int number, ...);
  void DblSet(int number, ...);
  void StrSet(int number, ...);
  void FlagSet(int number, ...);
  void IntListSet(int number, ...);
  void DblListSet(int number, ...);
  void StrListSet(int number, ...);

  inline void Process(ArgListType &argv, std::ostream & eout) {
    Process(argv, eout, cerr, false);
  }
  inline void Process(ArgListType &argv, std::ostream & eout,
                      std::ostream & sout) {
    Process(argv, eout, sout, true);
  }

  inline void HelpSet(const string& str) {
    HelpString = str;
  }
  void DisplayHelp(std::ostream & eout) const;

 private:
  void Process(ArgListType &argv, std::ostream & eout, std::ostream & sout,
               const bool dolog);

  vector<TArg<int> *> IntList;
  vector<TArg<double> *> DblList;
  vector<TArg<string> *> StrList;
  vector<FlagArg *> FlagList;
  vector<IntArgList *> IntListList;
  vector<DblArgList *> DblListList;
  vector<StrArgList *> StrListList;

  string FuncName;
  string HelpString;
};

inline ostream& operator << (ostream &os, const FlagArg &x) {
  return (os << x.getFlag());
}

template<typename T>
void setMatrixRange(int &startT, int &endT, int &startN, int &endN,
                    const TArg<int> &StartTime, const TArg<int> &EndTime,
                    const TArg<int> &StartNeuron, const TArg<int> &EndNeuron,
                    const vector<vector<T> > cfMtx, const CommandLine &ComL,
                    const string &FunctionName) {
  startN = StartNeuron.getValue();
  endN = EndNeuron.getValue();
  startT = StartTime.getValue();
  endT = EndTime.getValue();

  const int lastN = findMaxSize(cfMtx);
  const int lastT = cfMtx.size();

  if (endN == -1) {
    endN = lastN;
  }
  if (endT == -1) {
    endT = lastT;
  }
  // startN = endN + 1 implies an empty range - this is allowed
  if (endN > lastN || startN < 1 || startN > (endN+1)) {
    CALL_ERROR << "Error in " << FunctionName << ": Neuron range of "
               << startN << "..." << endN << " in a sequence with " << lastN
               << " neurons is invalid." << ERR_WHERE;
    ComL.DisplayHelp(Output::Err());
    exit(EXIT_FAILURE);
  }
  if (endT < 1 || endT > lastT || startT < 1 || startT > endT) {
    CALL_ERROR << "Error in " << FunctionName << ": Time range of "
               << startT << "..." << endT << " is invalid." << ERR_WHERE;
    ComL.DisplayHelp(Output::Err());
    exit(EXIT_FAILURE);
  }
}

#endif
