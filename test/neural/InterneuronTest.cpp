/***************************************************************************
 * InterneuronTest.cpp
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
#include "neural/Interneuron.hpp"

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
  class InterneuronTest : public ::testing::Test {
   protected:
    virtual void SetUp() {
      interneuron_constructed_ = Interneuron(0.9f, 3);
    }
    Interneuron interneuron_constructed_;
  };

  TEST_F(InterneuronTest, DefaultConstructorHasCorrectValues) {
    const Interneuron interneuron;
    EXPECT_FLOAT_EQ(1.0f, interneuron.getDecayRate());
    EXPECT_EQ(1, interneuron.getMaxTimeOffset());
  }

  TEST_F(InterneuronTest, CopyConstructorCopiesValues) {
    const Interneuron interneuron_dest(interneuron_constructed_);
    EXPECT_FLOAT_EQ(0.9f, interneuron_dest.getDecayRate());
    EXPECT_EQ(3, interneuron_dest.getMaxTimeOffset());
  }

  TEST_F(InterneuronTest, CompleteConstructorUsesValues) {
    const Interneuron interneuron(0.9f, 3);
    EXPECT_FLOAT_EQ(0.9f, interneuron.getDecayRate());
    EXPECT_EQ(3, interneuron.getMaxTimeOffset());
  }

  TEST_F(InterneuronTest, FaultyConstructorThrowsError) {
    try {
      const Interneuron faulty(0.9f, 0);
      FAIL() << "Should not be able to create neuron with zero buffer length";
    }
    catch (std::length_error e) {
      EXPECT_STREQ("Attempted to create an Interneuron with no buffer",
                   e.what());
    }
  }

  TEST_F(InterneuronTest, AssignmentOperatorCopiesValues) {
    Interneuron interneuron_dest;
    EXPECT_FLOAT_EQ(1.0f, interneuron_dest.getDecayRate());
    interneuron_dest = interneuron_constructed_;
    EXPECT_FLOAT_EQ(0.9f, interneuron_dest.getDecayRate());
    EXPECT_EQ(3, interneuron_dest.getMaxTimeOffset());
    interneuron_dest = interneuron_dest;
    EXPECT_FLOAT_EQ(0.9f, interneuron_dest.getDecayRate());
    EXPECT_EQ(3, interneuron_dest.getMaxTimeOffset());
  }

  TEST_F(InterneuronTest, DefaultCalcAndGetExcitationReturnsValue) {
    Interneuron interneuron;
    float expected = 1.234f;
    EXPECT_FLOAT_EQ(expected, interneuron.calcExcitation(expected));
    EXPECT_FLOAT_EQ(expected, interneuron.getExcitation());
    expected = 3.234f;
    EXPECT_FLOAT_EQ(expected, interneuron.calcExcitation(expected));
    EXPECT_FLOAT_EQ(expected, interneuron.getExcitation());
  }

  TEST_F(InterneuronTest, GetDecayRateReturnsWhatSetDecayRateSet) {
    Interneuron interneuron;
    const float decayRate = 0.25f;
    interneuron.setDecayRate(decayRate);
    EXPECT_FLOAT_EQ(decayRate, interneuron.getDecayRate());
  }

  TEST_F(InterneuronTest, WeightedCalcExcitationReturnsCorrectAverage) {
    Interneuron interneuron;
    const float decayRate = 0.25f;
    interneuron.setDecayRate(decayRate);
    const float val_1 = 1.234f;
    EXPECT_FLOAT_EQ(val_1, interneuron.calcExcitation(val_1));
    const float val_2 = 3.11f;
    const float expected = (1 - decayRate) * val_1 + decayRate * val_2;
    EXPECT_FLOAT_EQ(expected, interneuron.calcExcitation(val_2));
  }

  TEST_F(InterneuronTest, GetMultReturnsWhatSetMultSet) {
    Interneuron interneuron;
    const float mult = 0.25f;
    interneuron.setMult(mult);
    EXPECT_FLOAT_EQ(mult, interneuron.getMult());
  }

  TEST_F(InterneuronTest, GetMaxTimeOffsetReturnsWhatSetMaxTimeOffsetSet) {
    Interneuron interneuron;
    int expected = 1;
    interneuron.setMaxTimeOffset(expected);
    EXPECT_EQ(expected, interneuron.getMaxTimeOffset());
    expected = 3; // it can grow
    interneuron.setMaxTimeOffset(expected);
    EXPECT_EQ(expected, interneuron.getMaxTimeOffset());
    expected = 2; // it can shrink
    interneuron.setMaxTimeOffset(expected);
    EXPECT_EQ(expected, interneuron.getMaxTimeOffset());
  }
  
  TEST_F(InterneuronTest, SetMaxTimeOffsetThrowsErrorOnZero) {
    Interneuron interneuron;
    try {
      interneuron.setMaxTimeOffset(0);
      FAIL() << "Should not be able to set neuron to zero buffer length";
    }
    catch (std::length_error e) {
      EXPECT_STREQ("Attempted to alter Interneuron to have no buffer",
                   e.what());
    }
  }

  TEST_F(InterneuronTest, SetMultMultipliesCalcExcitation) {
    Interneuron interneuron;
    const float mult = 0.25f;
    interneuron.setMult(mult);
    const float val_1 = 1.234f;
    const float expected = val_1 * mult;
    EXPECT_FLOAT_EQ(expected, interneuron.calcExcitation(val_1));
  }
  
  TEST_F(InterneuronTest, ResetResetsInternalExcitation) {
    Interneuron interneuron;
    const float decayRate = 0.25f;
    interneuron.setDecayRate(decayRate);
    const float val_1 = 1.234f;
    EXPECT_FLOAT_EQ(val_1, interneuron.calcExcitation(val_1));
    interneuron.reset();
    const float expected = -1.f;
    EXPECT_FLOAT_EQ(expected, interneuron.getExcitation());
    const float val_2 = 3.11f;
    EXPECT_EQ(val_2, interneuron.calcExcitation(val_2));
  }

  TEST_F(InterneuronTest, GetInternrnWeightsHasSizeOfSetNumWeights) {
    Interneuron interneuron;
    const int numWeights = 3;
    const int firstN = 1;
    interneuron.setNumWeights(numWeights, firstN);
    EXPECT_EQ(numWeights, interneuron.getInternrnWeights().size());
  }
  
  TEST_F(InterneuronTest, FilteredCalcExcitationReturnsCorrectAverage) {
    Interneuron interneuron;
    DataList filterVals;
    const float f1 = 2.0f;
    const float f2 = 0.5f;
    filterVals.push_back(f1);
    filterVals.push_back(f2);
    interneuron.loadSynapseFilterValues(filterVals);
    interneuron.setMaxTimeOffset(filterVals.size());
    const float val_1 = 1.234f;
    EXPECT_FLOAT_EQ(0.0f, interneuron.calcExcitation(val_1));
    const float val_2 = 3.11f;
    float expected = val_1 * f1;
    EXPECT_FLOAT_EQ(expected, interneuron.calcExcitation(val_2));
    expected = val_2 * f1 + val_1 * f2;
    EXPECT_FLOAT_EQ(expected, interneuron.calcExcitation(0.0f));
  }
  
  TEST_F(InterneuronTest, CalcExcitationWithAfferentFiringsWorks) {
    Interneuron interneuron;
    const int numWeights = 4;
    const int firstN = 1;
    // Weights are initialized to 1.0
    interneuron.setNumWeights(numWeights, firstN);
    {
      UIVector afferentFirings;
      afferentFirings.push_back(1);
      afferentFirings.push_back(3);
      afferentFirings.push_back(4);
      afferentFirings.push_back(5);
      EXPECT_FLOAT_EQ(2.0f, interneuron.calcExcitation(afferentFirings, 1, 3));
    }
    {
      xInput afferentFirings(5);
      afferentFirings.turnOn(1);
      afferentFirings.turnOn(4);
      EXPECT_FLOAT_EQ(1.0f, interneuron.calcExcitation(afferentFirings, 1, 3));
    }
  }

#include <iostream>
  TEST_F(InterneuronTest, UpdateInternrnWeightsRespectSettings) {
    Interneuron interneuron;
    const int numWeights = 4;
    const int firstN = 1;
    // Weights are initialized to 1.0
    interneuron.setNumWeights(numWeights, firstN);
    UIVector afferentFirings;
    // 1-based
    afferentFirings.push_back(1);
    afferentFirings.push_back(2);
    afferentFirings.push_back(3);
    afferentFirings.push_back(4);
    UIVector justFired;
    // 1-based
    justFired.push_back(1);
    justFired.push_back(3);
    justFired.push_back(4);
    UIVector toModify;
    // 1-based
    toModify.push_back(1);
    toModify.push_back(2);
    toModify.push_back(3);
    toModify.push_back(4);
    EXPECT_FLOAT_EQ(4.0f, interneuron.calcExcitation(afferentFirings, 1, 4));
    const float desiredAct = 0.7f;
    interneuron.setDesiredActivity(desiredAct); // Actual is 0.75 above
    interneuron.updateInternrnWeights(justFired, toModify, 1, 4);
    // Should have no impact since m_SynModeRate is still 0
    EXPECT_FLOAT_EQ(4.0f, interneuron.calcExcitation(afferentFirings, 1, 4));
    const float synModRate = 0.1f;
    interneuron.setSynModRate(synModRate);
    float deviation = 0.75f - desiredAct;
    float new_weights = 1.0f + deviation * synModRate;
    float expected = 4 * new_weights;
    interneuron.updateInternrnWeights(justFired, toModify, 1, 4);
    EXPECT_FLOAT_EQ(expected,
                    interneuron.calcExcitation(afferentFirings, 1, 4));
    const float actAvgRate = 0.3f;
    interneuron.setActivityAveragingRate(actAvgRate);
    justFired.clear();
    justFired.push_back(1);
    justFired.push_back(4);
    float inst_deviation = 0.5f - desiredAct;
    deviation = deviation * actAvgRate + (1 - actAvgRate) * inst_deviation;
    const float unchangingWeight = new_weights;
    toModify.clear();
    // 1-based
    toModify.push_back(1);
    toModify.push_back(2);
    toModify.push_back(3);
    // Last neuron is no longer modified
    new_weights += deviation * synModRate;
    expected = 3 * new_weights + unchangingWeight;
    interneuron.updateInternrnWeights(justFired, toModify, 1, 4);
    EXPECT_FLOAT_EQ(expected,
                    interneuron.calcExcitation(afferentFirings, 1, 4));
    interneuron.setUseWeightedActAvg(true);
    float firedWeights = new_weights + unchangingWeight;
    float allWeights = 3 * new_weights + unchangingWeight;
    inst_deviation = firedWeights / allWeights - desiredAct;
    deviation = deviation * actAvgRate + (1 - actAvgRate) * inst_deviation;
    new_weights += deviation * synModRate;
    expected = 3 * new_weights + unchangingWeight;
    interneuron.updateInternrnWeights(justFired, toModify, 1, 4);
    EXPECT_FLOAT_EQ(expected,
                    interneuron.calcExcitation(afferentFirings, 1, 4));
    const float weightedActAvgAdj = 0.1f;
    interneuron.setWeightedActAvgAdj(weightedActAvgAdj);
    firedWeights = new_weights + unchangingWeight;
    allWeights = 3 * new_weights + unchangingWeight;
    inst_deviation = weightedActAvgAdj * firedWeights / allWeights - desiredAct;
    deviation = deviation * actAvgRate + (1 - actAvgRate) * inst_deviation;
    new_weights += deviation * synModRate;
    expected = 3 * new_weights + unchangingWeight;
    interneuron.updateInternrnWeights(justFired, toModify, 1, 4);
    EXPECT_FLOAT_EQ(expected,
                    interneuron.calcExcitation(afferentFirings, 1, 4));
    // gurantees all modified weights will hit zero
    interneuron.setWeightedActAvgAdj(-1000.0f);
    interneuron.updateInternrnWeights(justFired, toModify, 1, 4);
    EXPECT_FLOAT_EQ(unchangingWeight,
                    interneuron.calcExcitation(afferentFirings, 1, 4));
  }
}
