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
if(APPLE)
  # The following is necessary for working with XCode 4
  if(EXISTS /usr/bin/clang++)
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
  endif()
endif()    

cmake_minimum_required(VERSION 2.8)
project(NeuroJet_root)

# You must set GTEST_ROOT to the location of a directory containing an include and a lib directory,
# with the include directory having gtest/gtest.h and the lib directory having libgtest.a (or similar)
#message("GTEST_ROOT=$ENV{GTEST_ROOT}")

if(DEBUG)
   set(CMAKE_BUILD_TYPE Debug)
   add_definitions(--coverage -fno-inline-small-functions)
   add_definitions(-DCHECK_BOUNDS)
else()
   set(CMAKE_BUILD_TYPE Release)
endif()

if(MULTIPROC)
   add_definitions(-DMULTIPROC)
endif()

# Argg! Xcode currently does not work with _GLIBCXX_DEBUG (it compiles, but will easily crash)
if(APPLE)
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "DEBUG")
else()
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_GLIBCXX_DEBUG;DEBUG")
endif()

if(APPLE)
#   set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
   set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif()    

if(NOT WIN32)
   add_definitions(-Wall)
   set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g")
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
endif()

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")

set(SRC_DIR ${NeuroJet_root_SOURCE_DIR}/src/main/c++)
set(NEURAL_DIR ${SRC_DIR}/neural)
set(UTILS_DIR ${SRC_DIR}/utils)
set(SRC ${SRC_DIR}/ArgFuncts.cpp ${SRC_DIR}/Calc.cpp ${SRC_DIR}/NeuroJet.cpp
	${SRC_DIR}/Noise.cpp ${SRC_DIR}/Output.cpp ${SRC_DIR}/Parser.cpp ${SRC_DIR}/Population.cpp
	${SRC_DIR}/Program.cpp ${SRC_DIR}/SystemVar.cpp ${SRC_DIR}/rdtsc.s
	${NEURAL_DIR}/Interneuron.cpp ${NEURAL_DIR}/NeuronType.cpp
	${NEURAL_DIR}/DendriticSynapse.cpp ${NEURAL_DIR}/SynapseType.cpp ${UTILS_DIR}/StringUtils.cpp)
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
  	       ${NEURAL_DIR}/AxonalSynapse.hpp ${NEURAL_DIR}/DendriticSynapse.hpp ${NEURAL_DIR}/SynapseType.hpp
	       ${UTILS_DIR}/StringUtils.hpp)
endif()

enable_testing()

add_subdirectory(src)
