#include <string>
#include "gtest/gtest.h"

namespace {
  TEST(StringTest, EmptyStringIsEmpty) {
    const std::string EMPTYSTR(0, 'a');
    const std::string ANOTHEREMPTYSTR;
    EXPECT_STREQ(EMPTYSTR.c_str(), ANOTHEREMPTYSTR.c_str());
  }
}
