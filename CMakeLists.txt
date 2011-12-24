# Copyright (c) 2010-2011 Informed Simplifications, LLC
# All rights reserved.
#
# @since Jan 27, 2010
# @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>

if(APPLE)
# TODO: Add test for whether clang++ exists. If not, revert to g++
   if(EXISTS /usr/bin/clang++)
     set(CMAKE_CXX_COMPILER "clang++")
   endif()
endif()

cmake_minimum_required( VERSION 2.8 )
project(NeuroJet CXX)
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
endif()

include_directories(${NeuroJet_SOURCE_DIR} ${NeuroJet_SOURCE_DIR}/neural)

if(DEBUG)
   set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
endif()

file(GLOB ALL_SRC ${NeuroJet_SOURCE_DIR}/*.cpp ${NeuroJet_SOURCE_DIR}/neural/*.cpp ${NeuroJet_SOURCE_DIR}/*.s)
file(GLOB SRC ${NeuroJet_SOURCE_DIR}/*.cpp ${NeuroJet_SOURCE_DIR}/neural/*.cpp ${NeuroJet_SOURCE_DIR}/*.s)
if(NOT (MULTIPROC OR MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode"))
  list(REMOVE_ITEM SRC ${NeuroJet_SOURCE_DIR}/Parallel.cpp)
  list(REMOVE_ITEM SRC ${NeuroJet_SOURCE_DIR}/ParallelRand.cpp)
endif()

# Add header files so they show up in visual studio (really should be a 
# generator conditional test).
if(MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode")
  file(GLOB_RECURSE INCLUDES *.hpp)
endif()

set(FILES ${SRC} ${INCLUDES})
set(TEST_DIR ${NeuroJet_SOURCE_DIR}/Test)

enable_testing()

# Unit tests
add_executable(ArgFunctsTest ${TEST_DIR}/ArgFunctsTest.cpp ${NeuroJet_SOURCE_DIR}/Output.cpp)
add_test(ArgFunctsTest ArgFunctsTest)
add_executable(NoiseTest ${TEST_DIR}/NoiseTest.cpp ${NeuroJet_SOURCE_DIR}/Noise.cpp)
add_test(NoiseTest NoiseTest)
add_executable(StringTest ${TEST_DIR}/StringTest.cpp)
add_test(StringTest StringTest)

# Main executable
add_executable(NeuroJet ${FILES})
