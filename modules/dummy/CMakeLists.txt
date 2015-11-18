cmake_minimum_required(VERSION 3.3.0)
project(Celty)

include_directories(${CMAKE_SOURCE_DIR}/src/include)
include_directories(${PROJECT_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fno-builtin --param max-inline-insns-auto=100 --param early-inlining-insns=200")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-overflow=1 -Wformat=2 -Wuninitialized -Werror=return-type  -Werror=shadow -fstack-protector -Wformat-security")

file(GLOB SOURCES "./*.cc")


add_library(dummy SHARED ${SOURCES})
set_target_properties(dummy PROPERTIES PREFIX "")
set_target_properties(dummy PROPERTIES SUFFIX ${DEFAULT_MODULEEXT})

