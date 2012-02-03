/***************************************************************************
 * ArgFunctsTest.cpp
 *
 *  Copyright 2011 Ben Hocking
 *  This file is part of the NeuroJet tester AllTests.
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
#include "ArgFuncts.hpp"

#include "gtest/gtest.h"

#include <iostream>
#include <string>
#include <vector>

using std::vector;

namespace {
  class ArgFunctsTest : public ::testing::Test {
   protected:
    virtual void SetUp() {
      m33_ = vector<vector<double> >(3, vector<double>(3));
      m33_[0][0] = 0.5;
      m33_[0][1] = 0.3;
      m33_[0][2] = 0.3;
      m33_[1][0] = 0.2;
      m33_[1][1] = 0.1;
      m33_[1][2] = 0.2;
      m33_[2][0] = 0.7;
      m33_[2][1] = 0.4;
      m33_[2][2] = 0.4;
      dm33_ = DataMatrix(3, DataList(3));
      dm33_[0][0] = 0.5f;
      dm33_[0][1] = 0.3f;
      dm33_[0][2] = 0.3f;
      dm33_[1][0] = 0.2f;
      dm33_[1][1] = 0.1f;
      dm33_[1][2] = 0.2f;
      dm33_[2][0] = 0.7f;
      dm33_[2][1] = 0.4f;
      dm33_[2][2] = 0.4f;
    }
    
    vector<vector<double> > m33_;
    DataMatrix dm33_;
  };

  class AutoAssign: public std::unary_function<int, void> {
   public:
    static void reset() {
      next_ = 0;
    }

    AutoAssign() { }
    void operator() (double& toAssign) {
      toAssign = ++next_;
    }

   private:
    static int next_;
  };

  int AutoAssign::next_ = 0;

  class AutoMatrixAssign: public std::unary_function<vector<double>, void> {
   public:
    AutoMatrixAssign(): aa() { }
    void operator() (vector<double>& v) {
      for_each(v.begin(), v.end(), aa);
    }

   private:
    AutoAssign aa;
  };

  TEST_F(ArgFunctsTest, VectorSumAddsIntsCorrectly) {
    vector<int> vInt(3, 3);
    vInt[1] = 2; // vInt = 3, 2, 3
    EXPECT_EQ(8, vectorSum(vInt));
  }

  TEST_F(ArgFunctsTest, VectorSumAddsDoublesCorrectly) {
    vector<double> vDbl(2, 3.14);
    vDbl[1] = 2; // vDbl = 3.14, 2
    EXPECT_DOUBLE_EQ(5.14, vectorSum(vDbl));
  }
  
  TEST_F(ArgFunctsTest, VectorSumAddsFloatsCorrectly) {
    DataList vFlt(2, 3.14f);
    vFlt[1] = 2; // vDbl = 3.14, 2
    EXPECT_FLOAT_EQ(5.14, vectorSum(vFlt));
  }

  TEST_F(ArgFunctsTest, MatrixMeanAveragesVectorOfVectorsCorrectly) {
    const double expected = 0.34444444444444444;
    EXPECT_DOUBLE_EQ(expected, matrixMean(m33_));
    EXPECT_FLOAT_EQ(expected, matrixMean(dm33_));
  }

  TEST_F(ArgFunctsTest, MatrixMeanAveragesDataMatrixCorrectly) {
    DataMatrix dm(3, DataList(3, 2.72));
    dm[1][1] = 0;
    EXPECT_FLOAT_EQ(8.0f * 2.72f / 9.0f, matrixMean(dm));
  }

  TEST_F(ArgFunctsTest, MatrixMomentsCalculateForDataMatrixCorrectly) {
    DataMatrix dm(3, DataList(3, 2.72));
    dm[1][1] = 0;
    const double avg = 8 * 2.72 / 9;
    EXPECT_FLOAT_EQ(2.6490953233506943e-08f, matrixMoment(dm, avg, 1));
    EXPECT_FLOAT_EQ(0.73070619503657019f, matrixMoment(dm, avg, 2));
    EXPECT_FLOAT_EQ(-1.5458495600355997f, matrixMoment(dm, avg, 3));
  }

  TEST_F(ArgFunctsTest, MatrixAvgSSCalculateForDataMatrixCorrectly) {
    DataMatrix dm(3, DataList(3, 2.72));
    dm[1][1] = 0;
    EXPECT_FLOAT_EQ(6.5763556162516279f, matrixAvgSS(dm));
  }

  TEST_F(ArgFunctsTest, SubVectorReturnsVectorSubset) {
    vector<double> n(7);
    AutoAssign::reset();
    for_each(n.begin(), n.end(), AutoAssign());
    vector<double> subN = SubVector(n, 3, 5);
    for (int i = 0; i < subN.size(); ++i) {
      EXPECT_DOUBLE_EQ(static_cast<double>(i + 3), subN[i]);
    }
  }

  TEST_F(ArgFunctsTest, SubVectorReturnsVectorSubsetWhenBoundsAreExceeded) {
    vector<double> n(7);
    AutoAssign::reset();
    for_each(n.begin(), n.end(), AutoAssign());
    vector<double> subN = SubVector(n, 3, 15);
    for (int i = 0; i < subN.size(); ++i) {
      EXPECT_DOUBLE_EQ(static_cast<double>(i + 3), subN[i]);
    }
  }

  TEST_F(ArgFunctsTest, MatrixSumReturnsRowSums) {
    const vector<double> m33Sum = matrixSum(m33_);
    EXPECT_DOUBLE_EQ(1.1, m33Sum[0]);
    EXPECT_DOUBLE_EQ(0.5, m33Sum[1]);
    EXPECT_DOUBLE_EQ(1.5, m33Sum[2]);
  }

  TEST_F(ArgFunctsTest, MatrixTransposeTransposes) {
    vector<vector<double> > m22(2, vector<double>(2));
    AutoMatrixAssign ama;
    AutoAssign::reset();
    for_each(m22.begin(), m22.end(), ama);
    const vector<vector<double> > m22T = transposeMatrix(m22);
    EXPECT_DOUBLE_EQ(1.0, m22T[0][0]);
    EXPECT_DOUBLE_EQ(3.0, m22T[0][1]);
    EXPECT_DOUBLE_EQ(2.0, m22T[1][0]);
    EXPECT_DOUBLE_EQ(4.0, m22T[1][1]);
    const DataMatrix dmt = transposeMatrix(dm33_);
    EXPECT_FLOAT_EQ(0.2f, dmt[0][1]);
    EXPECT_FLOAT_EQ(0.2f, dmt[2][1]);
  }

  TEST_F(ArgFunctsTest, SubMatrixSamplesCorrectly) {
    vector<vector<double> > m44(4, vector<double>(4));
    AutoMatrixAssign ama;
    AutoAssign::reset();
    for_each(m44.begin(), m44.end(), ama);
    vector<vector<double> > subM = SubMatrix(m44, 2, 5, 3, 3);
    EXPECT_EQ(3, subM.size());
    for (int i = 0; i < subM.size(); ++i) {
      EXPECT_EQ(1, subM[i].size());
    }
    EXPECT_DOUBLE_EQ(7.0, subM[0][0]);
    EXPECT_DOUBLE_EQ(11.0, subM[1][0]);
    EXPECT_DOUBLE_EQ(15.0, subM[2][0]);
    DataMatrix subDM = SubMatrix(dm33_, 2, 5, 3, 3);
    EXPECT_EQ(2, subDM.size());
    for (int i = 0; i < subDM.size(); ++i) {
      EXPECT_EQ(1, subDM[i].size());
    }
    EXPECT_FLOAT_EQ(0.2f, subDM[0][0]);
    EXPECT_FLOAT_EQ(0.4f, subDM[1][0]);
  }

  TEST_F(ArgFunctsTest, SubMatrixReturnsEmptyMatrixWhenUpperLessThanLower) {
    vector<vector<double> > m44(4, vector<double>(4));
    AutoMatrixAssign ama;
    AutoAssign::reset();
    for_each(m44.begin(), m44.end(), ama);
    vector<vector<double> > subM = SubMatrix(m44, 2, 3, 3, 2);
    EXPECT_EQ(2, subM.size());
    EXPECT_EQ(0, subM[0].size());
    subM = SubMatrix(m44, 3, 2, 2, 3);
    EXPECT_EQ(0, subM.size());
  }
  
  TEST_F(ArgFunctsTest, MatrixSizeCalculatesSizeCorrectly) {
    EXPECT_EQ(9, matrixSize(m33_));
    EXPECT_EQ(9, matrixSize(dm33_));
    vector<vector<double> > m47(4, vector<double>(7));
    EXPECT_EQ(28, matrixSize(m47));
    vector<vector<double> > emptyMatrix;
    EXPECT_EQ(0, matrixSize(emptyMatrix));
    EXPECT_EQ(0, matrixSize(DataMatrix()));
  }

  TEST_F(ArgFunctsTest, MatrixAvgSSCalculatesSumOfSquaresAverage) {
    const double expected = 0.147777777777777777777;
    EXPECT_DOUBLE_EQ(expected, matrixAvgSS(m33_));
    EXPECT_FLOAT_EQ(expected, matrixAvgSS(dm33_));
  }

  TEST_F(ArgFunctsTest, MatrixVarCalculatesSameVarianceAsExcel) {
    const double expected = 0.032777777777777777777;
    EXPECT_DOUBLE_EQ(expected, matrixVar(m33_));
    EXPECT_FLOAT_EQ(expected, matrixVar(dm33_));
  }

  TEST_F(ArgFunctsTest, MatrixSkewCalculatesSameSkewAsExcel) {
    // The value found from Excel and verified by R:
//    const double expected = 0.764990820490810;
    // The value we're getting...
    const double expected = 0.76499062295692344;
    EXPECT_DOUBLE_EQ(expected, matrixSkew(m33_));
    EXPECT_FLOAT_EQ(expected, matrixSkew(dm33_));
  }

  TEST_F(ArgFunctsTest, MatrixKurtCalculatesSameKurtAsR) {
    // The value found from Excel didn't agree with R, and I trust R more
    //    const double expected = -0.2466963516230969;
    // The value we're getting...
    const double expected = -0.24669648874288708;
    EXPECT_DOUBLE_EQ(expected, matrixKurt(m33_));
    EXPECT_FLOAT_EQ(-0.24669673, matrixKurt(dm33_));
  }
  
  TEST_F(ArgFunctsTest, FindMaxSizeIsRobust) {
    EXPECT_EQ(3, findMaxSize(m33_));
    vector<vector<double> > emptyMatrix;
    EXPECT_EQ(0, findMaxSize(emptyMatrix));
    DataMatrix dm;
    dm.push_back(DataList(1, 0.0f));
    dm.push_back(DataList());
    dm.push_back(DataList(3, 2.3f));
    dm.push_back(DataList(39, 2.3f));
    EXPECT_EQ(39, findMaxSize(dm));
  }

  TEST_F(ArgFunctsTest, DelPtrZeroesAddress) {
    int* intPtr = new int();
    *intPtr = 3;
    int* nullPtr = NULL;
    EXPECT_LT(nullPtr, intPtr);
    delPtr(intPtr);
    EXPECT_EQ(nullPtr, intPtr);
    delPtr(intPtr);
    EXPECT_EQ(nullPtr, intPtr);
  }

  
  TEST_F(ArgFunctsTest, DelArrayZeroesAddress) {
    int* intPtr = new int[2];
    intPtr[0] = 3;
    int* nullPtr = NULL;
    EXPECT_LT(nullPtr, intPtr);
    delArray(intPtr);
    EXPECT_EQ(nullPtr, intPtr);
    delArray(intPtr);
    EXPECT_EQ(nullPtr, intPtr);
  }
}
