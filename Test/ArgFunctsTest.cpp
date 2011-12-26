#include "ArgFuncts.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"

using std::vector;

namespace {
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

  TEST(ArgFunctsTest, NestedTokensParseCorrectly) {
    const string s = "This is a ((nested (test)) string)";
    const char c = ' ';
    const string g = "()";
    const vector<string> tokens = tokenize(s, c, g);
    EXPECT_STREQ("This", tokens[0].c_str());
    EXPECT_STREQ("is", tokens[1].c_str());
    EXPECT_STREQ("a", tokens[2].c_str());
    EXPECT_STREQ("((nested (test)) string)", tokens[3].c_str());
  }

  TEST(ArgFunctsTest, VectorSumAddsIntsCorrectly) {
    vector<int> vInt(3, 3);
    vInt[1] = 2; // vInt = 3, 2, 3
    EXPECT_EQ(8, vectorSum(vInt));
  }

  TEST(ArgFunctsTest, VectorSumAddsDoublesCorrectly) {
    vector<double> vDbl(2, 3.14);
    vDbl[1] = 2; // vDbl = 3.14, 2
    EXPECT_DOUBLE_EQ(5.14, vectorSum(vDbl));
  }

  TEST(ArgFunctsTest, MatrixMeanAveragesVectorOfVectorsCorrectly) {
    vector<vector<double> > mDbl(3, vector<double>(3, 2.72));
    mDbl[1][1] = 0;
    EXPECT_DOUBLE_EQ(8 * 2.72 / 9, matrixMean(mDbl));
  }

  TEST(ArgFunctsTest, MatrixMeanAveragesDataMatrixCorrectly) {
    DataMatrix dm(3, DataList(3, 2.72));
    dm[1][1] = 0;
    EXPECT_FLOAT_EQ(8.0f * 2.72f / 9.0f, matrixMean(dm));
  }

  TEST(ArgFunctsTest, MatrixMomentsCalculateForDataMatrixCorrectly) {
    DataMatrix dm(3, DataList(3, 2.72));
    dm[1][1] = 0;
    const double avg = 8 * 2.72 / 9;
    EXPECT_DOUBLE_EQ(2.6490953430879927e-08, matrixMoment(dm, avg, 1));
    EXPECT_DOUBLE_EQ(0.73070621490478516, matrixMoment(dm, avg, 2));
    EXPECT_DOUBLE_EQ(-1.5458495616912842, matrixMoment(dm, avg, 3));
  }

  TEST(ArgFunctsTest, MatrixAvgSSCalculateForDataMatrixCorrectly) {
    DataMatrix dm(3, DataList(3, 2.72));
    dm[1][1] = 0;
    EXPECT_DOUBLE_EQ(6.5763564109802246, matrixAvgSS(dm));
  }

  TEST(ArgFunctsTest, SubVectorReturnsVectorSubset) {
    vector<double> n(7);
    AutoAssign::reset();
    for_each(n.begin(), n.end(), AutoAssign());
    vector<double> subN = SubVector(n, 3, 5);
    for (int i = 0; i < subN.size(); ++i) {
      EXPECT_DOUBLE_EQ(static_cast<double>(i + 3), subN[i]);
    }
  }

  TEST(ArgFunctsTest, SubVectorReturnsVectorSubsetWhenBoundsAreExceeded) {
    vector<double> n(7);
    AutoAssign::reset();
    for_each(n.begin(), n.end(), AutoAssign());
    vector<double> subN = SubVector(n, 3, 15);
    for (int i = 0; i < subN.size(); ++i) {
      EXPECT_DOUBLE_EQ(static_cast<double>(i + 3), subN[i]);
    }
  }

  TEST(ArgFunctsTest, MatrixSumReturnsColumnSums) {
    vector<vector<double> > m44(4, vector<double>(4));
    AutoMatrixAssign ama;
    AutoAssign::reset();
    for_each(m44.begin(), m44.end(), ama);
    const vector<double> m44Sum = matrixSum(m44);
    EXPECT_DOUBLE_EQ(10.0, m44Sum[0]);
    EXPECT_DOUBLE_EQ(26.0, m44Sum[1]);
    EXPECT_DOUBLE_EQ(42.0, m44Sum[2]);
    EXPECT_DOUBLE_EQ(58.0, m44Sum[3]);
  }

  TEST(ArgFunctsTest, MatrixTransposeTransposes) {
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

  TEST(ArgFunctsTest, SubMatrixSamplesCorrectly) {
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

  TEST(ArgFunctsTest, SubMatrixReturnsEmptyMatrixWhenUpperLessThanLower) {
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

  TEST(ArgFunctsTest, LTrimTrimsLeft) {
    EXPECT_STREQ("Test string 1 ", ltrim("\tTest string 1 ").c_str());
    EXPECT_STREQ("Test string 2", ltrim("Test string 2").c_str());
    EXPECT_EQ(0, ltrim("\t ").size());
  }

  TEST(ArgFunctsTest, RTrimTrimsRight) {
    EXPECT_STREQ("\tTest string 1", rtrim("\tTest string 1 ").c_str());
    EXPECT_STREQ("Test string 2", rtrim("Test string 2").c_str());
    EXPECT_EQ(0, rtrim("\t ").size());
  }

  TEST(ArgFunctsTest, UCaseUpperCases) {
    EXPECT_STREQ("\tTEST STRING 1 ", ucase("\tTest string 1 ").c_str());
    EXPECT_STREQ("TEST STRING 2", ucase("Test string 2").c_str());
    EXPECT_STREQ("\t ", ucase("\t ").c_str());
  }
}
