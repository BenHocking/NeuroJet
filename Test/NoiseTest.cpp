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
