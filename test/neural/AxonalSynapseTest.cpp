/***************************************************************************
 * AxonalSynapseTest.cpp
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
#include "neural/AxonalSynapse.hpp"
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
  class MockDendriticSynapse : public DendriticSynapse {
   public:
    MockDendriticSynapse()
      : DendriticSynapse(), wasActivated(false) { };
    virtual void activate(DataList &bus, DataList &bus_inhdiv,
                          DataList &bus_inhsub, const int timeStep) {
      wasActivated = true;
    }
    bool wasActivated;
  };

  TEST(AxonalSynapseTest, SynapseIsInitiallyUnconnected) {
    DendriticSynapse dummy;
    AxonalSynapse instance;
    EXPECT_FALSE(instance.connectsTo(dummy));
  }

  TEST(AxonalSynapseTest, ConnectedSynapseIsConnected) {
    DendriticSynapse receiver;
    AxonalSynapse instance;
    EXPECT_FALSE(instance.connectsTo(receiver));
    const SynapseType synapseType;
    instance.connectSynapse(1, receiver, &synapseType);
    EXPECT_TRUE(instance.connectsTo(receiver));
  }

  TEST(AxonalSynapseTest, GetWeightReturnsSetWeight) {
    DendriticSynapse receiver;
    AxonalSynapse instance;
    const SynapseType synapseType;
    instance.connectSynapse(1, receiver, &synapseType);
    const float expected = 0.35f;
    instance.setWeight(expected);
    EXPECT_FLOAT_EQ(expected, instance.getWeight());
  }

  TEST(AxonalSynapseTest, GetLastActivateReturnsSetLastActivate) {
    DendriticSynapse receiver;
    AxonalSynapse instance;
    const SynapseType synapseType;
    instance.connectSynapse(1, receiver, &synapseType);
    const int expected = 2;
    instance.setLastActivate(expected);
    EXPECT_FLOAT_EQ(expected, instance.getLastActivate());
  }
  
  TEST(AxonalSynapseTest, ActivateAxonalSynapseActivatesItsDendriticPart) {
    MockDendriticSynapse receiver;
    AxonalSynapse instance;
    const SynapseType synapseType;
    instance.connectSynapse(1, receiver, &synapseType);
    DataList fakeBus;
    EXPECT_FALSE(receiver.wasActivated);
    instance.activate(fakeBus, fakeBus, fakeBus, 1);
    EXPECT_TRUE(receiver.wasActivated);
  }
}
