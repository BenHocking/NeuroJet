# Copyright (c) 2010 Informed Simplifications, LLC
# All rights reserved.
#
# @since Jan 27, 2010
# @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>

cmake_minimum_required( VERSION 2.6 )
project(NEUROJET CXX)
if(DEBUG)
	set(CMAKE_BUILD_TYPE Debug)
	add_definitions(-DCHECK_BOUNDS)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

if(MULTIPROC)
	add_definitions(-DMULTIPROC)
endif()

set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_GLIBCXX_DEBUG;DEBUG")

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)
    set(CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
#    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GLIBCXX_FULLY_DYNAMIC_STRING")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -D_GLIBCXX_FULLY_DYNAMIC_STRING")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.5")
#    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.5")
    #Option -arch is only available with apple compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-arch i386" HAS_ARCH_FLAG)
#    if(HAS_ARCH_FLAG)
        if(${CMAKE_GENERATOR} STREQUAL "Xcode")
            set(CMAKE_OSX_ARCHITECTURES "i386;")
        else()
            # Build for single architecture. Using this flag requires more than one
            # architecture in the list, so this hack of only providing one forces
            # a single architecture compile.
            set(CMAKE_OSX_ARCHITECTURES "i386;i386")
        endif()
#    else()
#	message("Using 64-bit mode on Mac")
#    endif()
endif()    

if(NOT WIN32)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# Make sure the compiler can find include files. Need to find a better way of doing this
include_directories(${NEUROJET_SOURCE_DIR})

file(GLOB ALL_SRC ${NEUROJET_SOURCE_DIR}/*.cpp ${NEUROJET_SOURCE_DIR}/*.s)
file(GLOB SRC ${NEUROJET_SOURCE_DIR}/*.cpp ${NEUROJET_SOURCE_DIR}/*.s)
if(NOT (MULTIPROC OR MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode"))
  list(REMOVE_ITEM SRC ${NEUROJET_SOURCE_DIR}/Parallel.cpp)
  list(REMOVE_ITEM SRC ${NEUROJET_SOURCE_DIR}/ParallelRand.cpp)
endif()

# Add header files so they show up in visual studio (really should be a 
# generator conditional test).
if(MSVC OR ${CMAKE_GENERATOR} MATCHES "Xcode")
  file(GLOB_RECURSE INCLUDES *.hpp)
endif()

set(FILES ${SRC} ${INCLUDES})
set(TEST_DIR ${NEUROJET_SOURCE_DIR}/Test)

# Unit tests
add_executable(ArgFunctsTest ${TEST_DIR}/ArgFunctsTest.cpp ${NEUROJET_SOURCE_DIR}/Output.cpp)

# Main executable
add_executable(NeuroJet ${FILES})
