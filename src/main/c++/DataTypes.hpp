/***************************************************************************
 * DataTypes.hpp
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
#if !defined(DATATYPES_HPP)
#   define DATATYPES_HPP
///////////////////////////////////////////////////////////////////////////////
// System includes and defines
///////////////////////////////////////////////////////////////////////////////

#  if defined(MULTIPROC)
#     include <omp.h>
#  endif
#  include <algorithm>
#  include <climits>
#  include <cmath>
#  include <deque>
#  include <list>
#  include <map>
#  include <string>
#  include <utility>
#  include <vector>

#  if !defined(STRINGUTILS_HPP)
#    include "utils/StringUtils.hpp"
#  endif

using neurojet::stringutils::EMPTYSTR;

enum DataListType {
  DLT_unknown, DLT_sequence, DLT_matrix, DLT_analysis, DLT_file
};
const float verySmallFloat = 0.00000001f;
const float almostOne = 0.99999999f;

template<class T>
inline void updateMax(T &max, const T chk) {
  if (chk > max)
    max = chk;
}

typedef std::pair<unsigned int, unsigned int> UIPair;

#if defined(MEMORY)
#   define BOOL(X) X
typedef std::vector<bool> Pattern;
#else
#   define BOOL(X) (X != 0)
typedef std::vector<char> Pattern;
#endif
typedef std::vector<float> DataList;

typedef Pattern::iterator PatternIt;
typedef Pattern::const_iterator PatternCIt;
typedef DataList::iterator DataListIt;
typedef DataList::const_iterator DataListCIt;

// time x neuron
typedef std::vector<Pattern> Sequence;
typedef std::vector<DataList> DataMatrix;
typedef std::vector<std::string> StrList;

typedef Sequence::iterator SequenceIt;
typedef Sequence::const_iterator SequenceCIt;
typedef DataMatrix::iterator DataMatrixIt;
typedef DataMatrix::const_iterator DataMatrixCIt;
typedef StrList::iterator StrListIt;
typedef StrList::const_iterator StrListCIt;

typedef std::vector<unsigned int> UIVector;
typedef UIVector::iterator UIVectorIt;
typedef UIVector::const_iterator UIVectorCIt;

typedef std::vector<UIVector> UIMatrix;
typedef std::vector<UIVector> UIPtnSequence;
typedef UIMatrix::iterator UIMatrixIt;
typedef UIMatrix::const_iterator UIMatrixCIt;
typedef std::deque<UIVector> UIVectorDeque;
typedef UIPtnSequence::iterator UIPtnSequenceIt;
typedef UIPtnSequence::const_iterator UIPtnSequenceCIt;

typedef std::pair<std::string, bool> ArgType;
const ArgType EMPTYARG(EMPTYSTR, false);
typedef std::vector<ArgType> ArgListType;
typedef ArgListType::iterator ArgListTypeIt;
typedef ArgListType::const_iterator ArgListTypeCIt;
inline ArgListType StrToArgListType(const std::string& fromString) {
  return ArgListType(1, ArgType(fromString, false));
}

typedef void (*AT_FUN)(ArgListType &arg);
typedef std::string (*CARET_FUN)(ArgListType &arg);

/* Struct definition for competitive */
struct IxSumwz {
  IxSumwz(): ix(0), y(0) {}
  IxSumwz(int index, double val): ix(index), y(val) {}
  int ix;
  double y;
};
inline bool operator < (const IxSumwz &a, const IxSumwz &b) {
  return a.y < b.y;
}
inline bool operator > (const IxSumwz &a, const IxSumwz &b) {
  return a.y > b.y;
}

/* Formerly the contents of xInput.hpp: */
class xInput {
 public:
  inline xInput(): numOn(0) {}
  inline explicit xInput(const int numNeurons)
    : extPtn(Pattern(numNeurons, false)), numOn(0) {}
  inline xInput(const int numNeurons, const Pattern &inPtn):
    extPtn(inPtn), numOn(0) {
    numOn = inPtn.size() - std::count(inPtn.begin(), inPtn.end(), 0);
  };
  inline void initialize(const int numNeurons) {
    numOn = 0;
    extPtn = Pattern(numNeurons, false);
  }
#if defined(CHECK_BOUNDS)
  inline bool at(const int &index) const { return BOOL(extPtn.at(index)); }
  inline bool operator[] (const int &index) { return BOOL(extPtn.at(index)); }
  inline bool operator[] (const int &index) const {
    return BOOL(extPtn.at(index));
  }
#else
  inline bool at(const int &index) const { return BOOL(extPtn[index]); }
  inline bool operator[] (const int &index) { return BOOL(extPtn[index]); }
  inline bool operator[] (const int &index) const {
    return BOOL(extPtn[index]);
  }
#endif
  inline void turnOn(const int index) {
#if defined(CHECK_BOUNDS)
    extPtn.at(index) = true;
#else
    extPtn[index] = true;
#endif
    ++numOn;
  }
  inline void turnOff(const int index) {
#if defined(CHECK_BOUNDS)
    extPtn.at(index) = false;
#else
    extPtn[index] = false;
#endif
    --numOn;
  }
  inline int numExternals() const { return numOn; }
  inline Pattern::size_type size() const { return extPtn.size(); }

 private:
  Pattern extPtn;
  int numOn;
};

typedef std::vector<xInput> xInputList;
typedef xInputList::const_iterator xInputListCIt;
typedef xInputList::iterator xInputListIt;

inline DataList UIPtnToDataList(const UIVector &ptnIn,
                                unsigned int defaultNi = 0) {
  // assumes ptnIn is sorted
  unsigned int numNeurons = std::max((ptnIn.size() == 0) ? 0
                                     : (ptnIn.back() + 1), defaultNi);
  DataList toReturn(numNeurons);
  for (UIVectorCIt PCIt = ptnIn.begin(); PCIt != ptnIn.end(); PCIt++) {
    toReturn[*PCIt]=1.0f;
  }
  return toReturn;
}

inline UIVector DataListToUIPtn(const DataList &DLIn) {
  unsigned int ptnSize = DLIn.size();
  UIVector toReturn(0);
  for (unsigned int i = 0; i < ptnSize; ++i) {
    if (fabs(DLIn[i]) > verySmallFloat) {
      toReturn.push_back(i);
    }
  }
  return toReturn;
}

inline UIVector xInputToUIPtn(const xInput &xIn) {
  unsigned int ptnSize = xIn.size();
  UIVector toReturn(0);
  for (unsigned int i = 0; i < ptnSize; ++i) {
    if (xIn[i]) {
      toReturn.push_back(i);
    }
  }
  return toReturn;
}

inline DataMatrix UISeqToMatrix(const UIPtnSequence &seqIn,
                                unsigned int defaultNi = 0) {
  unsigned int mtxSize = seqIn.size();
  DataMatrix toReturn(mtxSize);
  DataMatrixIt DMIt = toReturn.begin();
  // (Willow) This is where the problem is
  for (UIPtnSequenceCIt SeqConstIterator = seqIn.begin();
       SeqConstIterator != seqIn.end(); SeqConstIterator++, DMIt++) {
    *DMIt = UIPtnToDataList(*SeqConstIterator, defaultNi);
  }
  return toReturn;
}

inline UIPtnSequence MatrixToUISeq(const DataMatrix &mtxIn) {
  unsigned int seqSize = mtxIn.size();
  UIPtnSequence toReturn(seqSize);
  UIPtnSequenceIt SIt = toReturn.begin();
  for (DataMatrixCIt DMCIt = mtxIn.begin(); DMCIt != mtxIn.end();
       DMCIt++, SIt++)
    *SIt = DataListToUIPtn(*DMCIt);
  return toReturn;
}

// Converts from a list of "neuron numbers" to a boolean vector (Pattern)
inline Pattern UIVectorToPtn(const UIVector &v, unsigned int defaultNi = 0) {
  unsigned int numNeurons = ((defaultNi == 0) && (v.size() > 0)) ? v.back() + 1
    : defaultNi;  // assumes v is sorted
  Pattern toReturn(numNeurons, false);
  for (UIVectorCIt it = v.begin(); it != v.end(); ++it) {
    toReturn[*it] = true;
  }
  return toReturn;
}

// Vice-versa of the above
inline UIVector PtnToUIVector(const Pattern &p) {
  UIVector toReturn(0);
  for (unsigned int i = 0; i < p.size(); ++i) {
    if (p[i]) toReturn.push_back(i);
  }
  return toReturn;
}

// Converts from a list of a list of "neuron numbers" to a list
// of boolean vectors (Patterns)
inline Sequence UISeqToPtnSeq(const UIPtnSequence &s,
                              unsigned int defaultNi = 0) {
  Sequence toReturn(0);
  for (UIPtnSequenceCIt it = s.begin(); it != s.end(); ++it) {
    toReturn.push_back(UIVectorToPtn(*it, defaultNi));
  }
  return toReturn;
}

// Vice-versa of the above
inline UIPtnSequence PtnSeqToUISeq(const Sequence &s) {
  UIPtnSequence toReturn(0);
  for (SequenceCIt it = s.begin(); it != s.end(); ++it) {
    toReturn.push_back(PtnToUIVector(*it));
  }
  return toReturn;
}

#endif
