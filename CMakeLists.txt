# Copyright (c) 2010-2011 Informed Simplifications, LLC
# All rights reserved.
#
# @since Jan 27, 2010
# @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>

if(APPLE)
   if(EXISTS /usr/bin/clang++)
     set(CMAKE_CXX_COMPILER "clang++")
     set(CMAKE_C_COMPILER "clang")
   endif()
endif()

cmake_minimum_required( VERSION 2.8 )
project(NeuroJet CXX)

find_package(GTest REQUIRED)

if(DEBUG)
   set(CMAKE_BUILD_TYPE Debug)
   add_definitions(--coverage)
   add_definitions(-DCHECK_BOUNDS)
else()
   set(CMAKE_BUILD_TYPE Release)
endif()

if(MULTIPROC)
   add_definitions(-DMULTIPROC)
endif()

if(APPLE)
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "DEBUG")
else()
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_GLIBCXX_DEBUG;DEBUG")
endif()

if(APPLE)
   set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif()    

if(NOT WIN32)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
   set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g")
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
endif()

include_directories(${NeuroJet_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})

if(DEBUG)
   set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
endif()

set(SRC_DIR ${NeuroJet_SOURCE_DIR})
set(NEURAL_DIR ${SRC_DIR}/neural)
set(UTILS_DIR ${SRC_DIR}/utils)
set(SRC ${SRC_DIR}/ArgFuncts.cpp ${SRC_DIR}/Calc.cpp ${SRC_DIR}/NeuroJet.cpp
	${SRC_DIR}/Noise.cpp ${SRC_DIR}/Output.cpp ${SRC_DIR}/Parser.cpp ${SRC_DIR}/Population.cpp
	${SRC_DIR}/Program.cpp ${SRC_DIR}/SystemVar.cpp ${SRC_DIR}/rdtsc.s
	${NEURAL_DIR}/Interneuron.cpp ${NEURAL_DIR}/NeuronType.cpp
	${NEURAL_DIR}/Synapse.cpp ${NEURAL_DIR}/SynapseType.cpp ${UTILS_DIR}/StringUtils.cpp)
if(MULTIPROC OR MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode")
  set(SRC ${SRC} ${SRC_DIR}/Parallel.cpp ${SRC_DIR}/ParallelRand.cpp)
endif()

# Add header files so they show up in visual studio (really should be a 
# generator conditional test).
if(MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode")
  set(INCLUDES ${SRC_DIR}/ActiveConnect.hpp ${SRC_DIR}/ArgFuncts.hpp ${SRC_DIR}/BindList.hpp
  	       ${SRC_DIR}/Calc.hpp ${SRC_DIR}/DataTypes.hpp ${SRC_DIR}/Filter.hpp
 	       ${SRC_DIR}/Matlab.hpp ${SRC_DIR}/MatrixMeanHelper.hpp ${SRC_DIR}/MatrixMomentHelper.hpp
  	       ${SRC_DIR}/MatrixSSHelper.hpp ${SRC_DIR}/MatrixSumHelper.hpp ${SRC_DIR}/NeuroJet.hpp
  	       ${SRC_DIR}/Noise.hpp ${SRC_DIR}/Output.hpp ${SRC_DIR}/Parallel.hpp ${SRC_DIR}/ParallelRand.hpp
  	       ${SRC_DIR}/Parser.hpp ${SRC_DIR}/Population.hpp ${SRC_DIR}/Program.hpp ${SRC_DIR}/SimState.hpp
  	       ${SRC_DIR}/State.hpp ${SRC_DIR}/Symbols.hpp ${SRC_DIR}/SystemVar.hpp ${SRC_DIR}/User.hpp
  	       ${SRC_DIR}/WeightAnalysis.hpp ${NEURAL_DIR}/Interneuron.hpp ${NEURAL_DIR}/NeuronType.hpp
  	       ${NEURAL_DIR}/Synapse.hpp ${NEURAL_DIR}/SynapseType.hpp ${UTILS_DIR}/StringUtils.hpp)
endif()

set(FILES ${SRC} ${INCLUDES})
set(TEST_DIR ${NeuroJet_SOURCE_DIR}/Test)

enable_testing()

# Unit tests
# * Requires FindGTest.cmake
# * Requires setting your GTEST_ROOT environment variable unless you're using another mechanism

add_executable(AllTests ${TEST_DIR}/TestHarness.cpp ${FILES} ${TEST_DIR}/NoiseTest.cpp ${TEST_DIR}/StringTest.cpp
			${TEST_DIR}/ArgFunctsTest.cpp ${TEST_DIR}/StringUtilsTest.cpp)
target_link_libraries(AllTests ${GTEST_BOTH_LIBRARIES})
add_test(AllTests AllTests)

# Main executable
add_executable(NeuroJet ${FILES} ${NeuroJet_SOURCE_DIR}/neurojet_driver.cpp)
