project(TwiserCpp)

cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0005 NEW)
set(BUILD_SHARED_LIBS ON)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CPP11_FLAGS "-std=c++11 -Wno-conversion-null")
set(COMPILER_FLAGS "-Wall -g ${CPP11_FLAGS}" CACHE STRING "common compiler
flags")
message("complier flags: " ${COMPILER_FLAGS})

set(MARCH "x86-64")

# Set the debug flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -Wno-attributes -march=${MARCH} ${COMPILER_FLAGS}" CACHE STRING "compiler options" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wno-attributes -march=${MARCH} ${COMPILER_FLAGS}" CACHE STRING "compiler options" FORCE)

set(CMAKE_C_FLAGS_RELEASE "-O3 -Wno-attributes -march=${MARCH} -mtune=${MTUNE} ${PROFILING_FLAGS} ${COMPILER_FLAGS}" CACHE STRING "compiler options" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ${WNO_LOCAL_TYPEDEFS} -Wno-attributes -march=${MARCH} -mtune=${MTUNE} ${PROFILING_FLAGS} ${COMPILER_FLAGS}" CACHE STRING "compiler options" FORCE)

#set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})

set(BOOST_HOME $ENV{HOME}/boost)

#set include path
include_directories (${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
	>   ${PROJECT_SOURCE_DIR}
	>   ${TwisterCpp_SOURCE_DIR}
	>   ${BOOST_HOME}/include)

message("boost home: ${BOOST_HOME}")

#set link path
link_directories(${BOOST_HOME}/lib)

subdirs(
	Twister-cpp
	)


