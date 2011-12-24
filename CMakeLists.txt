# Copyright (c) 2010-2011 Informed Simplifications, LLC
# All rights reserved.
#
# @since Jan 27, 2010
# @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>

cmake_minimum_required( VERSION 2.6 )
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

# Argg! Xcode currently does not work with _GLIBCXX_DEBUG (it compiles, but will easily crash)
if(APPLE)
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "DEBUG")
else()
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_GLIBCXX_DEBUG;DEBUG")
endif()

if(APPLE)
#    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)
#    set(CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
#    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -D_GLIBCXX_FULLY_DYNAMIC_STRING")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.5")
    #Option -arch is only available with apple compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-arch i386" HAS_ARCH_FLAG)
#    if(HAS_ARCH_FLAG) # For some 
        if(${CMAKE_GENERATOR} STREQUAL "Xcode")
            set(CMAKE_OSX_ARCHITECTURES "i386;")
        else()
            # Build for single architecture. Using this flag requires more than one
            # architecture in the list, so this hack of only providing one forces
            # a single architecture compile.
            set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
        endif()
#    else()
#	message("Using 64-bit mode on Mac")
#    endif()
endif()    

if(NOT WIN32)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

include_directories(${NeuroJet_SOURCE_DIR} ${NeuroJet_SOURCE_DIR}/neural)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")

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
