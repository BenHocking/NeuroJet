/***************************************************************************
 * SynapseTypeTest.cpp
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
#include "neural/SynapseType.hpp"

#include "gtest/gtest.h"

#include <cstdio>
#include <string>

using std::string;
using std::fopen;
using std::fclose;

namespace {
  const char RISE_FILE[] = "ZbarRiseArray.txt";
  const char FALL_FILE[] = "ZbarFallArray.txt";

  class SynapseTypeTest : public ::testing::Test {
   protected:
    virtual void SetUp() {
      s_type_constructed_ = SynapseType(LRT_MvgAvg, 0.01f, 2, 0.03f,
                                        0.04f, 0.05f, "pre", "post");
    }

    bool fileExists(const char* filename);
    void deleteIfExists(const char* filename);
    SynapseType s_type_constructed_;
  };

  bool SynapseTypeTest::fileExists(const char* filename) {
    FILE* check_file = fopen(filename, "r");
    bool retval = false;
    if (check_file != NULL) {
      fclose(check_file);
      retval = true;
    }
    return retval;
  }

  void SynapseTypeTest::deleteIfExists(const char* filename) {
    if (fileExists(filename)) {
      remove(filename);
    }
  }

  TEST_F(SynapseTypeTest, DefaultConstructorHasCorrectValues) {
    const SynapseType s_type;
    EXPECT_STREQ("default", s_type.getPreNeurType().c_str());
    EXPECT_STREQ("default", s_type.getPostNeurType().c_str());
    EXPECT_EQ(LRT_PostSyn, s_type.getLearningRule());
    EXPECT_FLOAT_EQ(0.0f, s_type.getSynModRate());
    EXPECT_EQ(1, s_type.getNMDArise());
    EXPECT_FLOAT_EQ(0.0f, s_type.getAlpha());
    EXPECT_FLOAT_EQ(1.0f, s_type.getKsyn());
    EXPECT_FLOAT_EQ(1.0f, s_type.getSynSuccRate());
    EXPECT_STREQ("", s_type.getRiseFile().c_str());
    EXPECT_EQ(0, s_type.getMaxTimeStep());
    const SynapseType s_type_named("my_name");
    EXPECT_STREQ("my_name", s_type_named.getPreNeurType().c_str());
    EXPECT_STREQ("my_name", s_type_named.getPostNeurType().c_str());
  }

  TEST_F(SynapseTypeTest, CopyConstructorCopiesValues) {
    const SynapseType s_type_dest(s_type_constructed_);
    EXPECT_STREQ(s_type_constructed_.getPreNeurType().c_str(),
                 s_type_dest.getPreNeurType().c_str());
    EXPECT_STREQ(s_type_constructed_.getPostNeurType().c_str(),
                 s_type_dest.getPostNeurType().c_str());
    EXPECT_FLOAT_EQ(s_type_constructed_.getSynModRate(), s_type_dest.getSynModRate());
    EXPECT_EQ(s_type_constructed_.getNMDArise(), s_type_dest.getNMDArise());
    EXPECT_FLOAT_EQ(s_type_constructed_.getAlpha(), s_type_dest.getAlpha());
    EXPECT_FLOAT_EQ(s_type_constructed_.getKsyn(), s_type_dest.getKsyn());
    EXPECT_FLOAT_EQ(s_type_constructed_.getSynSuccRate(), s_type_dest.getSynSuccRate());
    EXPECT_EQ(s_type_constructed_.getMaxTimeStep(), s_type_dest.getMaxTimeStep());
    EXPECT_STREQ(s_type_constructed_.getRiseFile().c_str(),
                 s_type_dest.getRiseFile().c_str());
  }

  TEST_F(SynapseTypeTest, CompleteConstructorUsesValues) {
    const SynapseType s_type(LRT_MvgAvg, 0.01f, 2, 0.03f, 0.04f,
                             0.05f, "pre", "post");
    EXPECT_STREQ("pre", s_type.getPreNeurType().c_str());
    EXPECT_STREQ("post", s_type.getPostNeurType().c_str());
    EXPECT_FLOAT_EQ(0.01f, s_type.getSynModRate());
    EXPECT_EQ(2, s_type.getNMDArise());
    EXPECT_FLOAT_EQ(0.03f, s_type.getAlpha());
    EXPECT_FLOAT_EQ(0.04f, s_type.getKsyn());
    // Constructor takes synaptic _failure_ rate
    EXPECT_FLOAT_EQ((1 - 0.05f), s_type.getSynSuccRate());
    EXPECT_EQ(SynapseType::MAX_TIME_STEP - 1, s_type.getMaxTimeStep());
    EXPECT_STREQ("", s_type.getRiseFile().c_str());
  }

  TEST_F(SynapseTypeTest, AssignmentOperatorCopiesValues) {
    SynapseType s_type_dest;
    EXPECT_STREQ("default", s_type_dest.getPreNeurType().c_str());
    s_type_dest = s_type_constructed_;
    EXPECT_STREQ(s_type_constructed_.getPreNeurType().c_str(),
                 s_type_dest.getPreNeurType().c_str());
    EXPECT_STREQ(s_type_constructed_.getPostNeurType().c_str(),
                 s_type_dest.getPostNeurType().c_str());
    EXPECT_FLOAT_EQ(s_type_constructed_.getSynModRate(), s_type_dest.getSynModRate());
    EXPECT_EQ(s_type_constructed_.getNMDArise(), s_type_dest.getNMDArise());
    EXPECT_FLOAT_EQ(s_type_constructed_.getAlpha(), s_type_dest.getAlpha());
    EXPECT_FLOAT_EQ(s_type_constructed_.getKsyn(), s_type_dest.getKsyn());
    EXPECT_FLOAT_EQ(s_type_constructed_.getSynSuccRate(), s_type_dest.getSynSuccRate());
    EXPECT_EQ(s_type_constructed_.getMaxTimeStep(), s_type_dest.getMaxTimeStep());
    EXPECT_STREQ(s_type_constructed_.getRiseFile().c_str(),
                 s_type_dest.getRiseFile().c_str());
  }

  TEST_F(SynapseTypeTest, GetAlphaReturnsWhatSetAlphaUsed) {
    SynapseType s_type;
    const float expected = 0.213f;
    s_type.setAlpha(expected);
    EXPECT_FLOAT_EQ(expected, s_type.getAlpha());
  }

  TEST_F(SynapseTypeTest, GetLearningRuleReturnsWhatSetLearningRuleUsed) {
    SynapseType s_type;
    const LearningRuleType expected = LRT_MvgAvg;
    s_type.setLearningRule(expected);
    EXPECT_EQ(expected, s_type.getLearningRule());
  }

  TEST_F(SynapseTypeTest, GetFilterReturnsWhatSetFilterUsed) {
    SynapseType s_type;
    Filter expected;
    DataList filterVals;
    filterVals.push_back(0.2f);
    filterVals.push_back(0.5f);
    expected.setFilter(filterVals);
    s_type.setFilter(filterVals);
    EXPECT_EQ(expected.getFilter(), s_type.getFilter()->getFilter());
  }

  TEST_F(SynapseTypeTest, GetKsynReturnsWhatSetKsynUsed) {
    SynapseType s_type;
    const float expected = 0.213f;
    s_type.setKsyn(expected);
    EXPECT_FLOAT_EQ(expected, s_type.getKsyn());
  }

  TEST_F(SynapseTypeTest, GetNMDAriseReturnsWhatSetNMDAriseUsed) {
    SynapseType s_type;
    // TODO(bhocking): Throw an error if this value is too large
    //  const unsigned int expected = 1234567;
    const unsigned int expected = 3;
    s_type.setNMDArise(expected);
    EXPECT_FLOAT_EQ(expected, s_type.getNMDArise());
  }

  TEST_F(SynapseTypeTest, GetSynModRateReturnsWhatSetSynModRateUsed) {
    SynapseType s_type;
    const float expected = 0.213f;
    s_type.setSynModRate(expected);
    EXPECT_FLOAT_EQ(expected, s_type.getSynModRate());
  }

  TEST_F(SynapseTypeTest, GetSynSuccRateCorrectForWhatSetSynFailRateUsed) {
    SynapseType s_type;
    const float expected = 0.213f;
    s_type.setSynFailRate(expected);
    EXPECT_FLOAT_EQ(1 - expected, s_type.getSynSuccRate());
  }

  TEST_F(SynapseTypeTest, SetSaveSavesOutputWhenAsked) {
    deleteIfExists(RISE_FILE);
    deleteIfExists(FALL_FILE);
    // Should only save file if saveIt == 1
    s_type_constructed_.setSave(0);
    EXPECT_FALSE(fileExists(RISE_FILE));
    EXPECT_FALSE(fileExists(FALL_FILE));
    s_type_constructed_.setSave(1);
    EXPECT_TRUE(fileExists(RISE_FILE));
    EXPECT_TRUE(fileExists(FALL_FILE));
    deleteIfExists(RISE_FILE);
    deleteIfExists(FALL_FILE);
  }
}
