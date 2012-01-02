/***************************************************************************
 * StringUtilsTest.cpp
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
#include "utils/StringUtils.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>

using neurojet::stringutils::from_string;
using neurojet::stringutils::ltrim;
using neurojet::stringutils::rtrim;
using neurojet::stringutils::to_string;
using neurojet::stringutils::tokenize;
using neurojet::stringutils::ucase;

using std::string;
using std::vector;

namespace {
  TEST(StringUtilsTest, LTrimTrimsLeft) {
    EXPECT_STREQ("Test string 1 ", ltrim("\tTest string 1 ").c_str());
    EXPECT_STREQ("Test string 2", ltrim("Test string 2").c_str());
    EXPECT_EQ(0, ltrim("\t ").size());
  }

  TEST(StringUtilsTest, NestedTokensParseCorrectly) {
    const string s = "This is a ((nested (test)) string)";
    const char c = ' ';
    const string g = "()";
    const vector<string> tokens = tokenize(s, c, g);
    EXPECT_STREQ("This", tokens[0].c_str());
    EXPECT_STREQ("is", tokens[1].c_str());
    EXPECT_STREQ("a", tokens[2].c_str());
    EXPECT_STREQ("((nested (test)) string)", tokens[3].c_str());
  }

  TEST(StringUtilsTest, RTrimTrimsRight) {
    EXPECT_STREQ("\tTest string 1", rtrim("\tTest string 1 ").c_str());
    EXPECT_STREQ("Test string 2", rtrim("Test string 2").c_str());
    EXPECT_EQ(0, rtrim("\t ").size());
  }

  TEST(StringUtilsTest, ToStringHandlesInt) {
    EXPECT_STREQ("1", to_string(1).c_str());
  }

  TEST(StringUtilsTest, ToStringHandlesDouble) {
    EXPECT_STREQ("1", to_string(1.0).c_str());
  }

  TEST(StringUtilsTest, ToStringHandlesVectorString) {
    vector<string> string_vector;
    string_vector.push_back("a");
    string_vector.push_back("string");
    EXPECT_STREQ("a string", to_string(string_vector).c_str());
  }

  TEST(StringUtilsTest, ToStringHandlesVectorDouble) {
    vector<double> double_vector;
    double_vector.push_back(1.0);
    double_vector.push_back(2.0);
    EXPECT_STREQ("1 2", to_string(double_vector).c_str());
  }

  TEST(StringUtilsTest, FromStringHandlesInt) {
    EXPECT_EQ(1, from_string<int>("1"));
  }

  TEST(StringUtilsTest, FromStringHandlesDouble) {
    EXPECT_DOUBLE_EQ(1.0, from_string<double>("1"));
  }

  TEST(StringUtilsTest, FromStringHandlesDoubleWithLeadingPeriod) {
    EXPECT_DOUBLE_EQ(0.01, from_string<double>(".01"));
  }

  TEST(StringUtilsTest, FromStringHandlesVectorString) {
    vector<string> string_vector = from_string<vector<string> >("a string");
    EXPECT_STREQ("a", string_vector[0].c_str());
    EXPECT_STREQ("string", string_vector[1].c_str());
  }

  TEST(StringUtilsTest, FromStringHandlesVectorDouble) {
    vector<double> double_vector = from_string<vector<double> >("1.0 2.1");
    EXPECT_DOUBLE_EQ(1.0, double_vector[0]);
    EXPECT_DOUBLE_EQ(2.1, double_vector[1]);
   }

  TEST(StringUtilsTest, FromStringHandlesVectorInt) {
    vector<int> int_vector = from_string<vector<int> >("1 21");
    EXPECT_EQ(1, int_vector[0]);
    EXPECT_EQ(21, int_vector[1]);
   }

  TEST(StringUtilsTest, UCaseUpperCases) {
    EXPECT_STREQ("\tTEST STRING 1 ", ucase("\tTest string 1 ").c_str());
    EXPECT_STREQ("TEST STRING 2", ucase("Test string 2").c_str());
    EXPECT_STREQ("\t ", ucase("\t ").c_str());
  }
}
