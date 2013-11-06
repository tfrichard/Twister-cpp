project(wordcount)

cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0005 NEW)

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

set(BOOST_HOME $ENV{HOME}/boost)

#set include path
include_directories($ENV{TWISTER_HOME}/)
message("Twister home: $ENV{TWISTER_HOME}")
include_directories(
	>   ${BOOST_HOME}/include)

message("boost home: ${BOOST_HOME}")

#set link path
link_directories(${BOOST_HOME}/lib)
link_directories($ENV{TWISTER_HOME}/lib)

set(SRC
	word_count_gather.cpp
	word_count_mapper.cpp
	word_count_reducer.cpp
	word_count_gather.h
	word_count_mapper.h
	word_count_reducer.h
)

add_library(wordcount SHARED
	${SRC})

set(BOOST_LIBS
	${BOOST_HOME}/lib/libboost_system.a
	${BOOST_HOME}/lib/libboost_thread.a
	${BOOST_HOME}/lib/libboost_filesystem.a
)
message("boost libs:  ${BOOST_LIBS}")
add_executable(tcc_wordcount
	main.cpp
	${SRC})
target_link_libraries(tcc_wordcount 
	$ENV{TWISTER_HOME}/lib/libtwistercpp.a
	${BOOST_LIBS}
	pthread
	dl)

install(TARGETS wordcount
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(TARGETS tcc_wordcount
	RUNTIME DESTINATION bin)
