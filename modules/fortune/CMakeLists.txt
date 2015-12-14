cmake_minimum_required(VERSION 2.8.0)
project(Celty)
option(BUILD_FORTUNE_MODULE "Build the Celty fortune Module" OFF)


if(BUILD_FORTUNE_MODULE)
	include_directories(${CMAKE_SOURCE_DIR}/src/include)
	include_directories(${PROJECT_BINARY_DIR})

	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fno-builtin")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3 -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-overflow=1 -Wformat=2 -Wuninitialized -Werror=return-type  -Werror=shadow -fstack-protector -Wformat-security")

	file(GLOB SOURCES "./*.cc")


	add_library(fortune SHARED ${SOURCES})
	set_target_properties(fortune PROPERTIES PREFIX "")
	set_target_properties(fortune PROPERTIES SUFFIX ${DEFAULT_MODULEEXT})

	install(TARGETS fortune DESTINATION ${DEFAULT_MODULEDIR})

endif()
