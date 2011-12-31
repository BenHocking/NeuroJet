/***************************************************************************
 * Calc.hpp
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
#if !defined(CALC_HPP)
#  define CALC_HPP

#  include <algorithm>
#  include <limits>
#  include <string>

#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif
#  if !defined(NOISE_HPP)
#    include "Noise.hpp"
#  endif
#  if !defined(SYSTEMVAR_HPP)
#    include "SystemVar.hpp"
#  endif

/**************************************************************/
/* CaretFunction Prototypes */
/**************************************************************/
std::string GetVar(ArgListType &arg);
std::string If(ArgListType &arg);
std::string CalcCaret(ArgListType &arg);

template<class T>
inline int sign(T val) {
  return (val < T(0)) ? -1 : 1;
}

/**************************************************************/
/* Quasi-private stuff used by Calc                           */
/**************************************************************/
#   if !defined(PI)
#      define PI 3.14159265358979323846
#   endif

struct CalcNode {
  // Constructors
  CalcNode() : prev(NULL), next(NULL), str(EMPTYSTR) {}
  explicit CalcNode(CalcNode * p) : prev(p), next(NULL), str(EMPTYSTR) {}
  explicit CalcNode(const std::string s) : prev(NULL), next(NULL), str(s) {}
  CalcNode(CalcNode * p, CalcNode * n) : prev(p), next(n), str(EMPTYSTR) {}
  CalcNode(CalcNode * p, const std::string s) : prev(p), next(NULL), str(s) {}
  CalcNode(CalcNode * p, CalcNode * n, const std::string s) : prev(p), next(n),
                                                              str(s) {}
  // Destructor
  // DON'T DELETE prev! (it should already be deleted!)
  ~CalcNode() {if (next != NULL) delete next;}
  // Modifier
  CalcNode *prev;
  CalcNode *next;
  std::string str;
};

class Calc {
 public:
  inline static void initCalc() {
    SystemVar::AddCaretFun("Calc", CalcCaret);
    SystemVar::AddCaretFun("GetVar", GetVar);
    SystemVar::AddCaretFun("If", If);
  }
  static double Calculate(const CalcNode * StartNode, CalcNode * &EndNode);
  inline static void ResetUserNoise(const int seed) {
    UserNoise.Reset(seed);
    isNoiseInit = true;
  }
  inline static string RandomUserSeed() {
    chkNoiseInit();
    return to_string(UserNoise.RandInt(1, 32767));
  }
  inline static float* convolve(const float* f1, unsigned int s1,
                                const float* f2, unsigned int s2,
                                const float dt = 1.0f) {
    float* toReturn = new float[s1 + s2 - 1]();
    for (unsigned int i = 0; i < s1; ++i) {
      for (unsigned int j = 0; j < s2; ++j) {
        toReturn[i+j] += f1[i] * f2[j] * dt;
      }
    }
    return toReturn;
  }
  inline static DataList convolve(const DataList &f1, const DataList &f2,
                                  const float dt = 1.0f) {
    const unsigned int f1size = f1.size();
    const unsigned int f2size = f2.size();
    DataList toReturn = DataList(f1size + f2size - 1, 0);
    for (unsigned int i = 0; i < f1size; ++i) {
      for (unsigned int j = 0; j < f2size; ++j) {
        toReturn[i+j] += f1[i] * f2[j] * dt;
      }
    }
    return toReturn;
  }

 private:
  inline static void chkNoiseInit() {
    if (!isNoiseInit) ResetUserNoise(SystemVar::GetIntVar("seed"));
  }
  static void getCalcArgs(const CalcNode * StartNode, CalcNode * &EndNode,
                          double &ArgOne, double &ArgTwo);
  static void getCalcArgs(const CalcNode * StartNode, CalcNode * &EndNode,
                          double &ArgOne, double &ArgTwo, double &ArgThree);
  inline static double getUserNormal(const double mu, const double sigma) {
    chkNoiseInit();
    return UserNoise.Normal(mu, sigma);
  }
  inline static double getUserRandInt(const unsigned int low,
                                      const unsigned int high) {
    chkNoiseInit();
    return UserNoise.RandInt(low, high);
  }
  inline static double getUserUniform(const double low, const double high) {
    chkNoiseInit();
    return UserNoise.Uniform(low, high);
  }
  static Noise UserNoise;  // random number generator for user specified rngs
  static bool isNoiseInit;
};

const std::string MathChars = ",+-*/%^!&|=<>~";                  // used by Calc
const std::string TokenChars = " \f\n\r\t\v[]{}()" + MathChars;  // "
const std::string MathStrings[] = {"*", "/", ",", "!", "%", "^", "&", "|", ">",
                                   "<", "=", "~|", ">=", "<=", "~=", "\\"};

void BuildCalcNodeList(CalcNode * &CurrentNode, const ArgListType &arg);
bool isinteger(const string s);
void CalcErr(const string msg);
double Calculate(const CalcNode * StartNode, CalcNode * &EndNode);
void errChkGeneric(const string curStr, const string nextStr);
void errChkMinus(const string nextStr);
void errChkMinusUnary(const string prevStr, const string nextStr);
void errChkNot(const string nextStr);
void errChkPlus(const string nextStr);
double myGamma(double Ex);
double gampdf(const double x, const double a, const double b);
bool isUnaryCompatible(const string prevStr);
double min(DataList arg);
double max(DataList arg);
void removeCurNode(CalcNode * &CurrentNode, CalcNode * &NextNode);
void removeNextNode(CalcNode * &CurrentNode, CalcNode * &NextNode);

/**************************************************************/
/*                    Some file functions                     */
/**************************************************************/

inline bool fileExists(const std::string filename) {
  std::ifstream exists(filename.c_str());
  if (!exists) return false;
  exists.close();
  return true;
}

#endif
