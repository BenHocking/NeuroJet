/***************************************************************************
 * SystemVar.hpp
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
#if !defined(SYSTEMVAR_HPP)
#  define SYSTEMVAR_HPP

#  include <map>
#  include <string>

#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif
#  if !defined(BINDLIST_HPP)
#    include "BindList.hpp"
#  endif
#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
#  if !defined(SIMSTATE_HPP)
#    include "SimState.hpp"
#  endif

struct Iterator {
  int CurrentVal;
  int StartVal;
  int EndVal;
  int StepVal;
};

template <class T> struct SystemData {
  T Data;
  bool IsReadOnly;
};

typedef SystemData<int> SysIntData;
typedef std::map<std::string, SysIntData> SysMapIntData;
typedef SystemData<float> SysFloatData;
typedef std::map<std::string, SysFloatData> SysMapFloatData;
typedef SystemData<std::string> SysStrData;
typedef std::map<std::string, SysStrData> SysMapStrData;
typedef SystemData<AT_FUN> SysAtData;
typedef std::map<std::string, SysAtData> SysMapAtData;
typedef SystemData<CARET_FUN> SysCaretData;
typedef std::map<std::string, SysCaretData> SysMapCaretData;
typedef SystemData<Iterator> SysItData;
typedef std::map<std::string, SysItData> SysMapItData;

class SystemVar {
 public:
  static void AddAtFun(std::string, AT_FUN);
  static void AddCaretFun(std::string, CARET_FUN);
  static void AddIntVar(std::string, int, const bool &ReadOnly = false);
  static void AddFloatVar(std::string, float, const bool &ReadOnly = false);
  static void AddSavedFile(std::string, std::string);
  static void AddStrVar(std::string, std::string, const bool &ReadOnly = false);

  inline static void ClearIntVar() {
    IntVar.clear();
  }
  inline static void ClearFloatVar() {
    FloatVar.clear();
  }
  inline static void ClearStrVar() {
    StrVar.clear();
  }
  inline static void ClearAllVars() {
    ClearIntVar();
    ClearFloatVar();
    ClearStrVar();
  }

  static void deleteData(const std::string& toDelete);

  static void exportVars(const std::string& fileName);

  static DataMatrix getAnalysis(const std::string& SeqName,
                                const std::string& FunctionName,
                                const CommandLine &ComL);
  inline static DataMatrix getAnalysis(const TArg<string>& SeqName,
                                       const std::string& FunctionName,
                                       const CommandLine &ComL) {
    return getAnalysis(SeqName.getValue(), FunctionName, ComL);
  }
  static DataMatrix & getAnalysis(const std::string& SeqName, StrList &anaDesc,
                                  const std::string& FunctionName,
                                  const CommandLine &ComL);
  static void getAnalysis(const std::string& SeqName, DataMatrix &anaList,
                          StrList &anaDesc, const std::string& FunctionName,
                          const CommandLine& ComL);

  inline static AT_FUN GetAtFun(const std::string& s) {
    return AtFunList.find(s)->second.Data;
  };

  inline static CARET_FUN GetCaretFun(const std::string& s) {
    return CaretFunList.find(s)->second.Data;
  };

  static DataMatrix getData(const std::string& SeqName,
                            const std::string& FunctionName,
                            const CommandLine& ComL);
  inline static DataMatrix getData(const TArg<std::string>& SeqName,
                                   const std::string& FunctionName,
                                   const CommandLine& ComL) {
    return getData(SeqName.getValue(), FunctionName, ComL);
  }

  inline static int GetIntVar(const std::string& s) {
    return IntVar.find(s)->second.Data;
  };

  inline static Iterator GetIterator(const std::string& s) {
    return IteratorList.find(s)->second.Data;
  };

  inline static float GetFloatVar(const std::string& s) {
    return FloatVar.find(s)->second.Data;
  };

  static DataMatrix getMatrix(const std::string& SeqName,
                              const std::string& FunctionName,
                              const CommandLine& ComL);
  inline static DataMatrix getMatrix(const TArg<std::string>& SeqName,
                                     const std::string& FunctionName,
                                     const CommandLine& ComL) {
    return getMatrix(SeqName.getValue(), FunctionName, ComL);
  }
  inline static DataMatrix getMatrixOrAnalysis(const std::string& varName,
                                               const std::string& FunctionName,
                                               const CommandLine& ComL) {
    const char varType = GetVarType(varName);
    if (varType == 'M') {
      return getMatrix(varName, FunctionName, ComL);
    }
    return getAnalysis(varName, FunctionName, ComL);
  }

  static UIPtnSequence getSequence(const std::string& SeqName,
                                   const std::string& FunctionName,
                                   const CommandLine& ComL);
  inline static UIPtnSequence getSequence(const TArg<std::string>& SeqName,
                                          const std::string& FunctionName,
                                          const CommandLine& ComL) {
    return getSequence(SeqName.getValue(), FunctionName, ComL);
  }

  inline static std::string GetStrVar(const std::string& s) {
    return StrVar.find(s)->second.Data;
  };

  inline static char GetVarType(const TArg<std::string>& s) {
    return GetVarType(s.getValue());
  };
  static char GetVarType(const std::string& s);
  static std::string GetVarTypeName(const std::string& s);

  static void IncIntVar(const std::string& varName, int incAmt = 1) {
    IntVar[varName].Data += incAmt;
  }

  static void insertAnalysis(const std::string& insertName,
                             const DataMatrix& insertData,
                             const StrList& insertDesc);

  static void insertData(const std::string& insertName,
                         const DataMatrix& toInsert,
                         DataListType insertType);
  inline static void insertData(const TArg<std::string>& insertName,
                                const DataMatrix& toInsert,
                                DataListType insertType) {
    insertData(insertName.getValue(), toInsert, insertType);
  }

  static void insertSequence(const std::string& seqName,
                             const UIPtnSequence& toInsert);
  inline static void insertSequence(const TArg<std::string>& seqName,
                                    const UIPtnSequence& toInsert) {
    insertSequence(seqName.getValue(), toInsert);
  }

  static bool IsReadOnly(const std::string& s);

  static void OutputAtFuns();
  static void OutputCaretFuns();
  static void OutputFloatVars();
  static void OutputIntVars();
  static void OutputStrVars();

  static void SetIntVar(const std::string& varName, int val);
  inline static void SetIterator(const std::string& VarName,
                                 const Iterator& iter) {
    IteratorList[VarName].Data = iter;
  }
  static void SetFloatVar(const std::string& varName, float val);
  static void SetStrVar(const std::string& varName, std::string val);

 private:
  static SysMapIntData IntVar;
  static SysMapFloatData FloatVar;
  static SysMapStrData StrVar;
  static SysMapStrData SavedFileList;
  static BindList<UIPtnSequence> SequenceList;  // list of sequences
  static BindList<DataMatrix> MatrixList;    // list of matrixes
  static BindList<DataMatrix> AnalysisList;  // list of data anlayses
  static BindList<StrList> AnalysisNames;    // names of analyses
  static BindList<SimState> SimStates;       // list of saved simulation states

  /* Function lists */
  static SysMapAtData AtFunList;
  static SysMapCaretData CaretFunList;

  /* Iterator list */
  static SysMapItData IteratorList;
};

#endif
