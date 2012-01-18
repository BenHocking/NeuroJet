/***************************************************************************
 * NeuronTypeTest.cpp
 *
 *  Copyright 2012 Ben Hocking
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
#include "neural/NeuronType.hpp"

#include "gtest/gtest.h"

#include <cstdio>
#include <map>
#include <stdexcept>
#include <string>

#include "SystemVar.hpp"

using std::fclose;
using std::fopen;
using std::map;
using std::string;

namespace {
  class NeuronTypeTest : public ::testing::Test {
   protected:
    virtual void SetUp() {
      n_type_constructed_ = NeuronType(false, true, TT_E, "constructed");
    }

    NeuronType n_type_constructed_;
  };

  TEST_F(NeuronTypeTest, DefaultConstructorHasCorrectValues) {
    const NeuronType n_type;
    EXPECT_STREQ(NeuronType::DEFAULT, n_type.getName().c_str());
    EXPECT_TRUE(n_type.isExcType());
    EXPECT_FALSE(n_type.isInhDivType());
    EXPECT_EQ(TT_Simple, n_type.getThresholdType());
    EXPECT_EQ(1, n_type.getFilterSize());
    const NeuronType n_type_named("my_name");
    EXPECT_STREQ("my_name", n_type_named.getName().c_str());
  }

  TEST_F(NeuronTypeTest, CopyConstructorCopiesValues) {
    const NeuronType n_type_dest(n_type_constructed_);
    EXPECT_STREQ(n_type_constructed_.getName().c_str(),
                 n_type_dest.getName().c_str());
    EXPECT_EQ(n_type_constructed_.isExcType(),
	      n_type_dest.isExcType());
    EXPECT_EQ(n_type_constructed_.isInhDivType(),
	      n_type_dest.isInhDivType());
    EXPECT_EQ(n_type_constructed_.getThresholdType(),
	      n_type_dest.getThresholdType());
    EXPECT_EQ(n_type_constructed_.getFilterSize(),
	      n_type_dest.getFilterSize());
  }

  TEST_F(NeuronTypeTest, CompleteConstructorUsesValues) {
    const NeuronType n_type(false, true, TT_E, "constructed");
    EXPECT_STREQ("constructed", n_type.getName().c_str());
    EXPECT_FALSE(n_type.isExcType());
    EXPECT_TRUE(n_type.isInhDivType());
    EXPECT_EQ(TT_E, n_type.getThresholdType());
    EXPECT_EQ(1, n_type.getFilterSize());
  }

  TEST_F(NeuronTypeTest, AssignmentOperatorCopiesValues) {
    NeuronType n_type_dest;
    EXPECT_STREQ(NeuronType::DEFAULT, n_type_dest.getName().c_str());
    n_type_dest = n_type_constructed_;
    EXPECT_STREQ(n_type_constructed_.getName().c_str(),
                 n_type_dest.getName().c_str());
    EXPECT_EQ(n_type_constructed_.isExcType(),
	      n_type_dest.isExcType());
    EXPECT_EQ(n_type_constructed_.isInhDivType(),
	      n_type_dest.isInhDivType());
    EXPECT_EQ(n_type_constructed_.getThresholdType(),
	      n_type_dest.getThresholdType());
    EXPECT_EQ(n_type_constructed_.getFilterSize(),
	      n_type_dest.getFilterSize());
  }

  TEST_F(NeuronTypeTest, GetThresholdTypeReturnsWhatSetThresholdTypeUsed) {
    NeuronType n_type;
    const ThresholdType expected = TT_Log;
    n_type.setThresholdType(expected);
    EXPECT_FLOAT_EQ(expected, n_type.getThresholdType());
  }

  TEST_F(NeuronTypeTest, HasParameterCannotFindMissingParameter) {
    NeuronType n_type;
    EXPECT_FALSE(n_type.hasParameter("dne"));
  }

  TEST_F(NeuronTypeTest, GetParameterReturnsDefaultForMissingParameter) {
    NeuronType n_type;
    int expected = 123;
    EXPECT_EQ(expected, n_type.getParameter("dne", expected));
  }

  TEST_F(NeuronTypeTest, GetParameterAndHasParameterWorkWithSetParameter) {
    NeuronType n_type;
    EXPECT_FALSE(n_type.hasParameter("testParameter"));
    n_type.setParameter("testParameter", "expected");
    EXPECT_TRUE(n_type.hasParameter("testParameter"));
    string surprise("surprise");
    EXPECT_STREQ("expected",
                 n_type.getParameter("testParameter", surprise).c_str());
  }

  TEST_F(NeuronTypeTest, GetFilterReturnsConvolvedFilter) {
    NeuronType n_type;
    DataList syn_filter;
    syn_filter.push_back(1.0f);
    syn_filter.push_back(1.0f);
    n_type.loadSynapseFilterValues(syn_filter);
    DataList dendr_filter;
    dendr_filter.push_back(1.0f);
    dendr_filter.push_back(1.0f);
    n_type.loadDTSFilterValues(dendr_filter);
    DataList defaultFilter;
    defaultFilter.push_back(1.0f);
    EXPECT_EQ(1, n_type.getFilter().size());
    EXPECT_EQ(defaultFilter, n_type.getFilter().getFilter());
    SystemVar::SetFloatVar("deltaT", 1.0f);
    n_type.loadDTSFilterValues(dendr_filter);
    DataList convolved;
    convolved.push_back(1.0f);
    convolved.push_back(2.0f);
    convolved.push_back(1.0f);
    EXPECT_EQ(convolved, n_type.getFilter().getFilter());
    NeuronType n_type_syn_only;
    n_type_syn_only.loadSynapseFilterValues(syn_filter);
    EXPECT_EQ(syn_filter, n_type_syn_only.getFilter().getFilter());
    NeuronType n_type_dendr_only;
    n_type_dendr_only.loadDTSFilterValues(dendr_filter);
    EXPECT_EQ(dendr_filter, n_type_dendr_only.getFilter().getFilter());
  }

  TEST_F(NeuronTypeTest, UzeIzhReturnsCorrectValue) {
    NeuronType n_type;
    EXPECT_FALSE(n_type.useIzh());
    n_type.setParameter("IzhA", "1");
    EXPECT_TRUE(n_type.useIzh());
  }
  
  TEST_F(NeuronTypeTest, ForceExtReturnsCorrectValue) {
    SystemVar::SetFloatVar("ExtExc", 0.0f);
    SystemVar::SetFloatVar("DGstrength", 0.0f);
    NeuronType n_type;
    EXPECT_TRUE(n_type.forceExt());
    n_type.setParameter("ExtExc", "1");
    EXPECT_FALSE(n_type.forceExt());
    n_type.setParameter("DGstrength", "1");
    EXPECT_FALSE(n_type.forceExt());
    n_type.setParameter("ExtExc", "0");
    EXPECT_FALSE(n_type.forceExt());
    n_type.setParameter("DGstrength", "0");
    EXPECT_TRUE(n_type.forceExt());
  }

  TEST_F(NeuronTypeTest, AddMemberAddsNeuronType) {
    EXPECT_EQ(0, NeuronType::Member.size());
    NeuronType::addMember("type_a", false, true, TT_Rational);
    EXPECT_EQ(1, NeuronType::Member.size());
    const NeuronType n_type(NeuronType::Member["type_a"]);
    EXPECT_STREQ("type_a", n_type.getName().c_str());
    EXPECT_FALSE(n_type.isExcType());
    EXPECT_TRUE(n_type.isInhDivType());
    EXPECT_EQ(TT_Rational, n_type.getThresholdType());
    EXPECT_EQ(1, n_type.getFilterSize());
    try {
      NeuronType::addMember("type_a", false, true, TT_Rational);
      FAIL() << "Should not be able to add the same member twice";
    }
    catch (std::runtime_error e) {
      EXPECT_STREQ("NeuronType 'type_a' already exists", e.what());
    }
  }
}
