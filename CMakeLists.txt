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
endif()

include_directories(${NeuroJet_SOURCE_DIR} ${NeuroJet_SOURCE_DIR}/neural ${GTEST_INCLUDE_DIRS})

if(DEBUG)
   set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
endif()

file(GLOB SRC ${NeuroJet_SOURCE_DIR}/*.cpp ${NeuroJet_SOURCE_DIR}/neural/*.cpp ${NeuroJet_SOURCE_DIR}/*.s)
list(REMOVE_ITEM SRC ${NeuroJet_SOURCE_DIR}/neurojet_driver.cpp)
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
# * Requires FindGTest.cmake
# * Requires setting your GTEST_ROOT environment variable unless you're using another mechanism

add_executable(AllTests ${TEST_DIR}/TestHarness.cpp ${FILES} ${TEST_DIR}/NoiseTest.cpp ${TEST_DIR}/StringTest.cpp
			${TEST_DIR}/ArgFunctsTest.cpp)
target_link_libraries(AllTests ${GTEST_BOTH_LIBRARIES})
add_test(AllTests AllTests)

# Main executable
add_executable(NeuroJet ${FILES} ${NeuroJet_SOURCE_DIR}/neurojet_driver.cpp)
