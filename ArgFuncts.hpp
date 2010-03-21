///////////////////////////////////////////////////////////////////////////////
// ArgFuncts.hpp
//
// prototypes and definitions for C++ function argument processing,
//
// Matt Harrison
//
// Last Update: 6/02/98
//
///////////////////////////////////////////////////////////////////////////////
//
// High Level Command Line Processing
//
// class IntArg(const string flag, const string help, int default)
// class IntArg(const string flag, const string help)
// class DblArg(const string flag, const string help, double default)
// class DblArg(const string flag, const string help)
// class StrArg(const string flag, const string help, string default)
// class StrArg(const string flag, const string help)
// class FlagArg(const string onflag, const string offflag, const string help, int default)
// class FlagArg(const string onflag, const string offflag, const string help)
//
// class CommandLine(string FunctionName)
// CommandLine.IntSet(int numints, IntArg * arg1, ...)
// CommandLine.DblSet(int numdbls, DblArg * arg1, ...)
// CommandLine.StrSet(int numstrs, StrArg * arg1, ...)
// CommandLine.FlagSet(int numflags, FlagArg * arg1, ...)
// CommandLine.HelpSet(string helpstr);
// CommandLine.Process(int argc, const string argv[], ostream& errorout)
// CommandLine.Process(int argc, const string argv[], ostream& errorout, ostream& callout)
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
#include <functional>
#include <cmath>
#include <cstdarg>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <cctype>
using std::cerr;
using std::cout;
using std::endl;
using std::list;
using std::map;
using std::ostream;
using std::pair;
using std::setw;
using std::string;
using std::vector;
using std::size_t;
using std::ptr_fun;

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

using std::min;
using std::max;
using std::find;
using std::transform;

////////////////////////////////////////////////////////////////////////////////
// Some matrix/vector functions (see also setMatrixRange, which depends on IntArg)
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
double matrixMoment(const vector<vector<T> > &Matrix, const float avg, const int moment) {
  return for_each(Matrix.begin(), Matrix.end(), MatrixMomentHelper<T>(avg, moment)).result();
}

template <typename T>
double matrixAvgSS(const vector<vector<T> > &Matrix) {
  return for_each(Matrix.begin(), Matrix.end(), MatrixSSHelper<T>()).result();
}

template <typename T>
double matrixVar(const vector<vector<T> > &Matrix) {
   const double avg = matrixMean(Matrix);
   return matrixAvgSS(Matrix) - avg * avg;
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
inline unsigned int findMaxSize(const T &dataIn, const unsigned int PriorMax = 0) {
   typename T::const_iterator it;
   unsigned int MaxSize = PriorMax;
   for (it = dataIn.begin(); it != dataIn.end(); it++) {
      updateMax(MaxSize, static_cast<unsigned int>(it->size()));
   }
   return MaxSize;
}

template<> // Template specialization
inline unsigned int findMaxSize(const UIPtnSequence &dataIn, const unsigned int PriorMax) {
   unsigned int MaxSize = PriorMax;
   for (UIPtnSequenceCIt it = dataIn.begin(); it != dataIn.end(); it++) {
      updateMax(MaxSize, it->back()+1); // assumes ptns in dataIn are sorted
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
   for (itSrcRow = toTranspose.begin(); itSrcRow != toTranspose.end(); ++itSrcRow, ++dstColCnt) {
      itDstRow = toReturn.begin();
      for (itSrcCol = itSrcRow->begin(); itSrcCol != itSrcRow->end(); ++itSrcCol, ++itDstRow) {
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
   vector<vector<T> > toReturn(oldColSize, vector<T>(oldRowSize, static_cast<T>(0)));
   itDstRow = toReturn.begin();
   unsigned int dstColCnt = 0;
   for (itSrcRow = toTranspose.begin(); itSrcRow != toTranspose.end(); ++itSrcRow, ++dstColCnt) {
      itDstRow = toReturn.begin();
      for (itSrcCol = itSrcRow->begin(); itSrcCol != itSrcRow->end(); ++itSrcCol, ++itDstRow) {
         (*itDstRow)[dstColCnt] = *itSrcCol;
      }
   }
   return toReturn;
}

// first & last are 1-based, VecIn & toReturn are 0-based
template<typename T>
inline vector<T> SubVector(const vector<T> &VecIn, unsigned int first, unsigned int last) {
  unsigned int useLast = last == 0 ? VecIn.size() : min(last, static_cast<unsigned int>(VecIn.size()));
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
// Some string manipulation functions
////////////////////////////////////////////////////////////////////////////////
inline string ltrim(string const& source, char const* delims = " \t\r\n") {
  string result(source);
  string::size_type index = result.find_first_not_of(delims);
  if (index != string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}

inline string rtrim(string const& source, char const* delims = " \t\r\n") {
  string result(source);
  string::size_type index = result.find_last_not_of(delims);
  if (index != string::npos)
    result.erase(++index);
  else
    result.erase();
  return result;
}

inline string ucase(const string &s) {
  string toReturn(EMPTYSTR);
  transform(s.begin(), s.end(), back_inserter(toReturn), ptr_fun<int, int>(toupper));
  return toReturn;
}

template<typename T>
inline string to_string(const T& data_in) {
   std::ostringstream os;
   os << std::setprecision(20) << data_in;
   return os.str();
}

template<>
inline string to_string(const vector<string>& data_in) {
   std::ostringstream os;
   os << std::setprecision(20);
   for (unsigned int i = 0; i < data_in.size(); i++) {
     os << data_in.at(i);
   }
   return os.str();
}

template<>
inline string to_string(const vector<double>& data_in) {
   std::ostringstream os;
   os << std::setprecision(20);
   for (unsigned int i = 0; i < data_in.size(); i++) {
     os << data_in.at(i);
   }
   return os.str();
}

template<>
inline string to_string(const vector<int>& data_in) {
   std::ostringstream os;
   os << std::setprecision(20);
   for (unsigned int i = 0; i < data_in.size(); i++) {
     os << data_in.at(i);
   }
   return os.str();
}

template<typename T>
inline T from_string(const string &data_in) {
   if (data_in[0] == '.')
      return from_string<T>("0" + data_in);
   std::istringstream is(data_in);
   T data_out;
   is >> data_out;
   return data_out;
}

template<> // abh2n: Hack to get code to compile. Don't actually use this!
inline vector<string> from_string(const string &data_in) {
  vector<string> data_out;
  return data_out;
}

template<> // abh2n: Hack to get code to compile. Don't actually use this!
inline vector<double> from_string(const string &data_in) {
  vector<double> data_out;
  return data_out;
}

template<> // abh2n: Hack to get code to compile. Don't actually use this!
inline vector<int> from_string(const string &data_in) {
  vector<int> data_out;
  return data_out;
}

// Helper function for tokenize. Not used anywhere else
inline void addToStrVec(vector<string>& container, const string& s, int begin, int end) {
  // Only adds conditionally
  if (end > begin) {
    container.push_back(s.substr(begin, end - begin));
  }
}

// tokenize separates the string s using token, except where token is inside of
// elements specified by groups. For example, if groups is ""()[], then the
// token will be ignored when it is inside quotes, parentheses, or square
// brackets.
inline vector<string> tokenize(const string &s, const char token,
                               const string &groups) {
   vector<string> toReturn(0, EMPTYSTR);
   const unsigned int numGrps = groups.size() / 2;
   vector<int> grpCnt = vector<int>(numGrps, 0);
   const string combo = groups + token;
   string::size_type prev = -1;
   string::size_type start = 0;
   string::size_type found = s.find_first_of(combo);
   bool insideGroup = false;
   while (found != string::npos) {
     if (s[found] == token) {
       if (!insideGroup) { // Do NOT combine with previous if statement
	 addToStrVec(toReturn, s, start, found);
	 start = found + 1;
       }
     }
     else {
       string::size_type grp = groups.find(s[found]);
       int& numInGrp = grpCnt[grp / 2];
       if (grp % 2 == 0) {
	 numInGrp++;
       }
       else {
	 numInGrp--;
       }
       insideGroup = (numInGrp > 0);
       if (numInGrp < 0) {
	 CALL_ERROR << "Mismatching " << groups[grp] << ERR_WHERE;
	 exit(EXIT_FAILURE);
       }
     }
     prev = found;
     found = s.find_first_of(combo, prev + 1);
   }
   addToStrVec(toReturn, s, start, s.size());
   return toReturn;
}

////////////////////////////////////////////////////////////////////////////////
// Some data deallocation functions
////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline void delPtr(T &ptr)
{
   if (ptr) {
      delete ptr;
      ptr = NULL;
   }
}

template<typename T>
inline void delArray(T &ptr)
{
   if (ptr) {
      delete[] ptr;
      ptr = NULL;
   }
}

template<typename T>
inline void delMatrix(T &ptr, int height)
{
   if (ptr) {
      for (int i=0; i<height; i++)
         delArray(ptr[i]);
      delete[] ptr;
      ptr = NULL;
   }
}

template<typename T>
inline void delTensor(T &ptr, int otherDim, int height)
{
   if (ptr) {
      for (int i=0; i<otherDim; i++)
         delMatrix(ptr[i], height);
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
                 vector<T> &Value, const bool IsDeault, const vector<T> Deault) {
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
      assignpos++; // Move to the first item in the list (after the #)
      for (int j = 0; j < numvals; j++, assignpos++) {
         // If the argument is already accounted for
         if (assignpos->second) {
            return "Unable to find " + to_string(numvals) + " arguments after " + Flag;
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
   return EMPTYSTR; // No errMsg
}

template<typename T>
inline void printTHelp(ostream &os, const string &Flag, const string &Help,
                       const bool IsDeault, const T &Deault)
{
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
  TArg(const string& flag, const string& help, const T defaultVal): _value(defaultVal), _flag(flag), _help(help),
								    _defaultVal(defaultVal), _isDefault(true) { }
  TArg(const string& flag, const string& help): _value(0), _flag(flag), _help(help), _defaultVal(0), _isDefault(false) { }
  virtual ~TArg() { }
  const string& getFlag() const { return _flag; }
  const T& getValue() const { return _value; }
  virtual ostream& printHelp(ostream& os) const {
   int w = 10;
   const string s = " : ";
   os << std::setw(w) << _flag;
   os << s;
   os << std::setw(w) << (_isDefault ? to_string<T>(_defaultVal) : "{required}");
   os << s << _help << std::endl;
   return os << std::setw(w) << _flag << s << std::setw(w)
	     << (_isDefault ? to_string<T>(_defaultVal) : "{required}")
	     << s << _help << "\n";
  }
  virtual bool setValue(const T& newValue) {
    _value = newValue;
    return true;
  }
  void swapValue(TArg& otherArg) { std::swap(_value, otherArg._value); }
  virtual bool setValue(ArgListType &argv) {
   // Find an argument that matches the flag, but that hasn't already been found
   ArgListTypeIt thing1 = find(argv.begin(), argv.end(), ArgType(_flag, false));
   if (thing1 != argv.end()) {
      // Check out its corresponding value
      ArgListTypeIt thing2 = thing1 + 1;
      // If we're not at the end and not already marked as being found
      if ((thing2 != argv.end()) && (!thing2->second)) {
         // Mark it and its value as being accounted for
         thing1->second = true;
         thing2->second = true;
         _value = from_string<T>(thing2->first);
         return true;
      }
   }
   if (!_isDefault) return false;
   _value = _defaultVal;
   return true;
  }
protected:
  T _value;
  string _flag;
  string _help;
  T _defaultVal;
  bool _isDefault;
};

// Template specialization for strings
template <>
inline TArg<string>::TArg(const string& flag, const string& help): _value(EMPTYSTR), _flag(flag), _help(help), _defaultVal(EMPTYSTR), _isDefault(false) { }

template <typename T>
ostream& operator<<(ostream &os, const TArg<T> &x) {
   return (os << x.getFlag() << " " << x.getValue());
}

class IntArgList: public TArg<vector<int> > {
public:
  IntArgList(const string& flag, const string& help): TArg<vector<int> >(flag, help), _errMsg(EMPTYSTR) { }
  inline const string& getErr() const { return _errMsg; }
  inline int getValue(const int i) const { return _value.at(i); }
  ostream& printHelp(ostream &os) const {
    printListHelp(os, _flag, _help, _isDefault, _defaultVal);
    return os;
  }
  bool setValue(ArgListType &argv) {
    _errMsg = setListValue(argv, _flag, _value, _isDefault, _defaultVal);
    return _errMsg.size() == 0;
  }
  inline unsigned int size() const { return _value.size(); }
  inline int& operator[] (int index) { return _value.at(index);}
  inline const int& operator[] (int index) const { return _value.at(index);}

private:
   string _errMsg;
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
  DblArgList(const string& flag, const string& help): TArg<vector<double> >(flag, help), _errMsg(EMPTYSTR) { }
  inline const string& getErr() const { return _errMsg; };
  inline double getValue(const int i) const { return _value.at(i); };
  ostream& printHelp(ostream &os) const {
     printListHelp(os, _flag, _help, _isDefault, _defaultVal);
     return os;
  };
  bool setValue(ArgListType &argv) {
     _errMsg = setListValue(argv, _flag, _value, _isDefault, _defaultVal);
     return _errMsg.size() == 0;
  };
  inline unsigned int size() const { return _value.size(); };
  inline double &operator[] (int index) { return _value[index]; };
  inline const double &operator[] (int index) const { return _value[index]; };

 protected:
   string _errMsg;
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
   StrArgList(const string& flag, const string& help, const bool isOptional = false):
     TArg<vector<string> >(flag, help), _isOptional(isOptional), _errMsg(EMPTYSTR) { }
   inline const string& getErr() const { return _errMsg; }
   inline const string& getValue(const int i) const { return _value.at(i); }
   ostream& printHelp(ostream &os) const {
      printListHelp(os, _flag, _help, _isOptional, _defaultVal);
      return os;
   }
   bool setValue(ArgListType &argv) {
      _errMsg = setListValue(argv, _flag, _value, _isOptional, _defaultVal);
      return _errMsg.size() == 0;
   }
   inline unsigned int size() const {return _value.size();}
   inline string& operator[] (int index) { return _value.at(index); };
   inline const string& operator[] (int index) const { return _value.at(index); };

 private:
   bool _isOptional;
   string _errMsg;
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
   FlagArg(const string& onFlag, const string& offFlag, const string& help, bool defaultVal):
     TArg<bool>(onFlag, help, defaultVal), _onFlag(onFlag), _offFlag(offFlag) { }
   FlagArg(const string& onFlag, const string& offFlag, const string& help):
     TArg<bool>(onFlag, help), _onFlag(onFlag), _offFlag(offFlag) { }
   ostream& printHelp(ostream &os) const;
   inline bool setValue(bool newValue) { _value = newValue; return true; };
   bool setValue(ArgListType &argv);

 protected:
   string _onFlag;
   string _offFlag;
};

class CommandLine {
 public:
   CommandLine(const string funcname): FuncName(funcname), HelpString(EMPTYSTR) {};
   ~CommandLine() {};

   void IntSet(int number, ...);
   void DblSet(int number, ...);
   void StrSet(int number, ...);
   void FlagSet(int number, ...);
   void IntListSet(int number, ...);
   void DblListSet(int number, ...);
   void StrListSet(int number, ...);

   inline void Process(ArgListType &argv, std::ostream & eout) {
      Process(argv, eout, cerr, false);
   };
   inline void Process(ArgListType &argv, std::ostream & eout,
                       std::ostream & sout) {
      Process(argv, eout, sout, true);
   };

   inline void HelpSet(const string& str) {
      HelpString = str;
   };
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
