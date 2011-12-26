/***************************************************************************
 * NoiseTest.cpp
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
#include "Noise.hpp"
#include <iostream>
#include <stdexcept>
#include "gtest/gtest.h"

namespace {
  TEST(NoiseTest, RandIntRespectsBounds) {
    long unsigned int seed = 4380L;
    Noise instance(seed);
    int low = 0;
    int high = 2047;
    for (int i = 0; i < 10000000; ++i) {
      int val = instance.RandInt(low, high);
      EXPECT_GE(val, low) << "Generated value " << val << " should be greater than or equal to  " << low;
      EXPECT_LE(val, high) << "Generated value " << val << " should be less than or equal to " << high;
    }
  }
}
