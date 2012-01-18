/***************************************************************************
 * StringTest.cpp
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
#include <string>
#include "gtest/gtest.h"

namespace {
  TEST(StringTest, EmptyStringIsEmpty) {
    const std::string EMPTYSTR(0, 'a');
    const std::string ANOTHEREMPTYSTR;
    EXPECT_STREQ(EMPTYSTR.c_str(), ANOTHEREMPTYSTR.c_str());
  }
}
