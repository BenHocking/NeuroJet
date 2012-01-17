#  Copyright (c) 2012 Ben hocking
#  All rights reserved.
#
#  @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>
#
#  This file is part of NeuroJet.
#
#  NeuroJet is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  NeuroJet is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with NeuroJet.  If not, see <http://www.gnu.org/licenses/lgpl.txt>.

find_package(GTest REQUIRED)

include_directories(${SRC_DIR} ${GTEST_INCLUDE_DIRS})

set(FILES ${SRC} ${INCLUDES})
set(TEST_DIR ${NeuroJet_parent_SOURCE_DIR}/test)

enable_testing()

# Unit tests
# * Requires FindGTest.cmake
# * Requires setting your GTEST_ROOT environment variable unless you're using another mechanism

add_executable(AllTests ${TEST_DIR}/TestHarness.cpp ${FILES} ${TEST_DIR}/NoiseTest.cpp ${TEST_DIR}/StringTest.cpp
			${TEST_DIR}/ArgFunctsTest.cpp
			${TEST_DIR}/neural/InterneuronTest.cpp
			${TEST_DIR}/neural/AxonalSynapseTest.cpp ${TEST_DIR}/neural/DendriticSynapseTest.cpp
			${TEST_DIR}/neural/NeuronTypeTest.cpp ${TEST_DIR}/neural/SynapseTypeTest.cpp
			${TEST_DIR}/utils/StringUtilsTest.cpp)
target_link_libraries(AllTests ${GTEST_BOTH_LIBRARIES})
add_test(AllTests AllTests)
