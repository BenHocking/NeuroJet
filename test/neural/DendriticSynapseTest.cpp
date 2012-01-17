/***************************************************************************
 * DendriticSynapseTest.cpp
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
#include "neural/DendriticSynapse.hpp"
#include "neural/SynapseType.hpp"

#include "gtest/gtest.h"

#include <cstdio>
#include <map>
#include <stdexcept>
#include <string>

using std::fclose;
using std::fopen;
using std::map;
using std::string;

namespace {
  TEST(DendriticSynapseTest, GetSrcNeuronReturnsWhatSetSrcNeuronSet) {
    DendriticSynapse instance;
    EXPECT_EQ(0, instance.getSrcNeuron());
    const unsigned int expected = 33215;
    instance.setSrcNeuron(expected);
    EXPECT_EQ(expected, instance.getSrcNeuron());
  }

  TEST(DendriticSynapseTest, GetWeightReturnsWhatSetWeightSet) {
    DendriticSynapse instance;
    EXPECT_FLOAT_EQ(0.0f, instance.getWeight());
    const float expected = 0.3f;
    instance.setWeight(expected);
    EXPECT_FLOAT_EQ(expected, instance.getWeight());
  }

  TEST(DendriticSynapseTest, GetLastActivateReturnsWhatSetLastActivateSet) {
    DendriticSynapse instance;
    EXPECT_EQ(DendriticSynapse::NEVER_ACTIVATED, instance.getLastActivate());
    const int expected = 13;
    instance.setLastActivate(expected);
    EXPECT_EQ(expected, instance.getLastActivate());
  }

  TEST(DendriticSynapseTest, ResetLastActivateDoesSo) {
    DendriticSynapse instance;
    const int expected = 13;
    instance.setLastActivate(expected);
    EXPECT_EQ(expected, instance.getLastActivate());
    instance.resetLastActivate();
    EXPECT_EQ(DendriticSynapse::NEVER_ACTIVATED, instance.getLastActivate());
  }

  TEST(DendriticSynapseTest, ConnectNeuronSetsSynapseType) {
    DendriticSynapse instance;
    const SynapseType expected;
    instance.connectNeuron(1, &expected, true, false);
    EXPECT_EQ(&expected, instance.getSynapseType());
  }

  TEST(DendriticSynapseTest, CalcZBarWorksDuringNMDARise) {
    DendriticSynapse instance;
    SynapseType synapseType;
    instance.connectNeuron(1, &synapseType, true, false);
    synapseType.setNMDArise(5);  // Can be done before or after connecting
    DataList bus;
    DataList bus_inhdiv;
    DataList bus_inhsub;
    bus.push_back(0.0f);
    bus.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    // Synapse is activated on time step 1, so it should rise until time step 5
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    float expected = 0.2f;
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(2, 1));
    expected = 0.0f;
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(2, 2));
    // Test PostSynB rule
    synapseType.setLearningRule(LRT_PostSynB);
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(2, 2));
    instance.resetLastActivate();
    expected = 0.2f;
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(2, 2));
  }

  TEST(DendriticSynapseTest, CalcZBarWorksForDoubleFireDuringNMDARise) {
    DendriticSynapse instance;
    SynapseType synapseType;
    const float alpha = exp(-1.0f / 120);
    synapseType.setAlpha(alpha);
    instance.connectNeuron(1, &synapseType, true, false);
    synapseType.setNMDArise(5);  // Can be done before or after connecting
    DataList bus;
    DataList bus_inhdiv;
    DataList bus_inhsub;
    bus.push_back(0.0f);
    bus.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    // Synapse is activated on time step 1, so it should rise until time step 5
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    // Fire again during rise (should now rise until time step 7)
    instance.activate(bus, bus_inhdiv, bus_inhsub, 3);
    float expected = 0.6f;
    // Even though last activated is on time step 3, we started the rise on time
    //  step 1, so with an NMDA rise of 5, each time step increases by 0.2
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(4, 3));
    expected = 1.0f;
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(6, 3));
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(7, 3));
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(8, 3));
    // Now the fall begins
    EXPECT_FLOAT_EQ(alpha, instance.calcZBar(9, 3));
    EXPECT_FLOAT_EQ(alpha * alpha, instance.calcZBar(10, 3));
  }

  TEST(DendriticSynapseTest, UpdateWeight) {
    DendriticSynapse instance;
    SynapseType synapseType;
    const float alpha = exp(-1.0f / 120);
    synapseType.setAlpha(alpha);
    const float mu = 0.05f;
    synapseType.setSynModRate(mu);
    instance.connectNeuron(1, &synapseType, true, false);
    synapseType.setNMDArise(5);  // Can be done before or after connecting
    float weight = 0.4f;
    instance.setWeight(weight);
    instance.updateWeight(1);
    weight -= mu * weight;
    EXPECT_FLOAT_EQ(weight, instance.getWeight());
    DataList bus;
    DataList bus_inhdiv;
    DataList bus_inhsub;
    bus.push_back(0.0f);
    bus.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    // Synapse is activated on time step 1, so it should rise until time step 5
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    // Fire again during rise (should now rise until time step 7)
    instance.activate(bus, bus_inhdiv, bus_inhsub, 3);
    float expectedZBar = 0.6f;
    weight += mu * (expectedZBar - weight);
    instance.updateWeight(4);
    EXPECT_FLOAT_EQ(weight, instance.getWeight());
    // Test moving averager
    synapseType.setLearningRule(LRT_MvgAvg);
    // Because we just now turned on the moving average, its value is 0
    weight -= mu * weight;
    instance.updateWeight(4);
    EXPECT_FLOAT_EQ(weight, instance.getWeight());
    // Test multi-activity post-synaptic
    synapseType.setLearningRule(LRT_MultiActPS);
    // Both activities happen on rise, so both use same value on curve
    weight += mu * (0.6f - weight);
    weight += mu * (0.6f - weight);
    instance.updateWeight(4);
    EXPECT_FLOAT_EQ(weight, instance.getWeight());
  }

  TEST(DendriticSynapseTest, ActivateFillsAppropriateBus) {
    DendriticSynapse instance;
    SynapseType synapseType;
    const float alpha = exp(-1.0f / 120);
    synapseType.setAlpha(alpha);
    const float mu = 0.05f;
    synapseType.setSynModRate(mu);
    instance.connectNeuron(1, &synapseType, true, false);
    synapseType.setNMDArise(10);  // Can be done before or after connecting
    const float kSyn = 0.3f;
    synapseType.setKsyn(kSyn);
    const float weight = 0.4f;
    instance.setWeight(weight);
    DataList bus;
    DataList bus_inhdiv;
    DataList bus_inhsub;
    bus.push_back(0.0f);
    bus.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    // Synapse is activated on time step 1, so it should rise until time step 5
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    float expected = kSyn * weight;
    EXPECT_FLOAT_EQ(expected, bus[1]);
    EXPECT_FLOAT_EQ(0.0f, bus_inhdiv[1]);
    EXPECT_FLOAT_EQ(0.0f, bus_inhsub[1]);
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    expected += kSyn * weight;
    EXPECT_FLOAT_EQ(expected, bus[1]);
    instance.connectNeuron(1, &synapseType, false, true);
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    expected = kSyn * weight;
    EXPECT_FLOAT_EQ(2 * expected, bus[1]);
    EXPECT_FLOAT_EQ(expected, bus_inhdiv[1]);
    EXPECT_FLOAT_EQ(0.0f, bus_inhsub[1]);
    instance.connectNeuron(1, &synapseType, false, false);
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    expected = kSyn * weight;
    EXPECT_FLOAT_EQ(2 * expected, bus[1]);
    EXPECT_FLOAT_EQ(expected, bus_inhdiv[1]);
    EXPECT_FLOAT_EQ(expected, bus_inhsub[1]);
  }

  TEST(DendriticSynapseTest, ActivateTwiceDuringRiseThenDuringFall) {
    DendriticSynapse instance;
    SynapseType synapseType;
    const float alpha = exp(-1.0f / 120);
    synapseType.setAlpha(alpha);
    const float mu = 0.05f;
    synapseType.setSynModRate(mu);
    instance.connectNeuron(1, &synapseType, true, false);
    synapseType.setNMDArise(10);  // Can be done before or after connecting
    const float kSyn = 0.3f;
    synapseType.setKsyn(kSyn);
    const float weight = 0.4f;
    instance.setWeight(weight);
    DataList bus;
    DataList bus_inhdiv;
    DataList bus_inhsub;
    bus.push_back(0.0f);
    bus.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    // Synapse is activated on time step 1, so it should rise until time step 10
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    // Fire again during rise (should now rise until time step 12)
    instance.activate(bus, bus_inhdiv, bus_inhsub, 3);
    // Fire yet again during rise (should now rise until time step 13)
    instance.activate(bus, bus_inhdiv, bus_inhsub, 4);
    float expected = 0.4f;
    // Even though last activated is on time step 4, we started the rise on time
    //  step 1, so with an NMDA rise of 10, each time step increases by 0.1
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(5, 4));
    // Now the fall begins
    EXPECT_FLOAT_EQ(alpha, instance.calcZBar(15, 4));
    instance.activate(bus, bus_inhdiv, bus_inhsub, 15);
    expected = 1.0f; // It quickly gets back to its max
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(16, 15));
    // Stays at max for ten time-steps
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(17, 15));
    EXPECT_FLOAT_EQ(expected, instance.calcZBar(25, 15));
    // Now the fall begins again
    EXPECT_FLOAT_EQ(alpha, instance.calcZBar(26, 15));
  }

  TEST(DendriticSynapseTest, MovingAveragerAverages) {
    DendriticSynapse instance;
    const float mu = 0.05f;
    const unsigned int NMDArise = 10;
    const float alpha = exp(-1.0f / 120);
    const float inv_alpha = 1 - alpha;
    const float kSyn = 0.3f;
    SynapseType synapseType(LRT_MvgAvg, mu, NMDArise, alpha, kSyn, 0.0f,
                            "default", "default");
    instance.connectNeuron(1, &synapseType, true, false);
    float weight = 0.4f;
    instance.setWeight(weight);
    DataList bus;
    DataList bus_inhdiv;
    DataList bus_inhsub;
    bus.push_back(0.0f);
    bus.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhdiv.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    bus_inhsub.push_back(0.0f);
    // Synapse is activated on time step 1, so it should rise until time step 10
    instance.activate(bus, bus_inhdiv, bus_inhsub, 1);
    float mvgAvg = inv_alpha;
    instance.activate(bus, bus_inhdiv, bus_inhsub, 3);
    float expectedZBar = 0.3f;
    mvgAvg = expectedZBar * mvgAvg + inv_alpha;
    EXPECT_FLOAT_EQ(expectedZBar, instance.calcZBar(4, 3));
    instance.updateWeight(4);
    weight += mu * (expectedZBar * mvgAvg - weight);
    EXPECT_FLOAT_EQ(weight, instance.getWeight());
    EXPECT_FLOAT_EQ(alpha, instance.calcZBar(14, 3));
    instance.activate(bus, bus_inhdiv, bus_inhsub, 14);
  }
}
