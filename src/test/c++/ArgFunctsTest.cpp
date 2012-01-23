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
    }
    
    vector<vector<double> > m33_;
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

  TEST_F(ArgFunctsTest, MatrixMeanAveragesVectorOfVectorsCorrectly) {
    const double expected = 0.34444444444444444;
    EXPECT_DOUBLE_EQ(expected, matrixMean(m33_));
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
    EXPECT_DOUBLE_EQ(2.6490953430879927e-08, matrixMoment(dm, avg, 1));
    EXPECT_DOUBLE_EQ(0.73070621490478516, matrixMoment(dm, avg, 2));
    EXPECT_DOUBLE_EQ(-1.5458495616912842, matrixMoment(dm, avg, 3));
  }

  TEST_F(ArgFunctsTest, MatrixAvgSSCalculateForDataMatrixCorrectly) {
    DataMatrix dm(3, DataList(3, 2.72));
    dm[1][1] = 0;
    EXPECT_DOUBLE_EQ(6.5763564109802246, matrixAvgSS(dm));
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
    vector<vector<double> > emptyMatrix;
    EXPECT_EQ(0, matrixSize(emptyMatrix));
  }

  TEST_F(ArgFunctsTest, MatrixAvgSSCalculatesSumOfSquaresAverage) {
    const double expected = 0.147777777777777777777;
    EXPECT_DOUBLE_EQ(expected, matrixAvgSS(m33_));
  }

  TEST_F(ArgFunctsTest, MatrixVarCalculatesSameVarianceAsExcel) {
    const double expected = 0.032777777777777777777;
    EXPECT_DOUBLE_EQ(expected, matrixVar(m33_));
  }
}
