cmake_minimum_required(VERSION 2.8.0)
project(Celty)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug, Release" FORCE)
endif()


message(STATUS "Current Build Type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(PROJECT_ETC_DIR ${CMAKE_SOURCE_DIR}/etc)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(MODULE_DIR ${CMAKE_SOURCE_DIR}/modules)
option(BUILD_MODULES "Build the modules with Celty" ON)


if (CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
					OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_LESS 5.1)
			message(FATAL_ERROR "Need GCC Version 5.1 or newer")
	endif()
endif()

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")


add_subdirectory(src)
if(BUILD_MODULES)
	add_subdirectory(modules)
endif()

