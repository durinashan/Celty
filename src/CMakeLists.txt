cmake_minimum_required(VERSION 2.8.0)
project(celty)

include(CheckLibraryExists)
include(CheckIncludeFiles)
include(FindPkgConfig)
include(GetGitVersion)

find_package(LibEV REQUIRED)
find_package(Threads REQUIRED)

get_git_version(GIT_VERSION)

include_directories(include)

set(DAEMON_NAME "celty" CACHE STRING "The name of the daemon")
set(DEFAULT_RUNAS "nobody" CACHE STRING "The default user to run as")
set(DEFAULT_LOCKFILE "${DAEMON_NAME}.lwk" CACHE STRING "The default PID lockfile name")
set(DEFAULT_LOCKDIR "/var/lock/subsys/" CACHE PATH "The default lock directory")
set(DEFAULT_WORKINGDIR "/" CACHE PATH "The default working directory")
set(DEFAULT_MODULEDIR "./" CACHE STRING "The relative path to look for modules in")
set(DEFAULT_MODULEEXT ".moe" CACHE STRING "The default extension for modules")
set(CONFIG_DIRECTORY "/etc/" CACHE STRING "The default configuratio file directory")
set(CONFIG_FILE "celty.cfg" CACHE STRING "The name of the config file")
set(PID_FILE "/tmp/celty.pid" CACHE STRING "The pid file celty will use for signaling")

configure_file ("${PROJECT_ETC_DIR}/celty-config.hh.in"
                "${PROJECT_BINARY_DIR}/celty-config.hh")

include_directories(${PROJECT_BINARY_DIR})


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fno-builtin --param max-inline-insns-auto=100 --param early-inlining-insns=200")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3 -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-overflow=1 -Wformat=2 -Wuninitialized -Werror=return-type  -Werror=shadow -fstack-protector -Wformat-security")


file(GLOB SOURCES "./*.cc")

add_executable(celty ${SOURCES})


target_link_libraries(celty dl ev)
