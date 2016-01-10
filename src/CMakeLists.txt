cmake_minimum_required(VERSION 2.8.0)
project(celty)

include(CheckLibraryExists)
include(CheckIncludeFiles)
include(FindPkgConfig)
include(GetGitVersion)

find_package(LibEV REQUIRED)
find_package(Threads REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	include(systemd)
endif()

get_git_version(GIT_VERSION)

include_directories(include)

option(AFL_BUILD "Builds with the american fuzzy lop shim and runs as a fuzz test" OFF)

set(DAEMON_NAME "celty" CACHE STRING "The name of the daemon")
set(DEFAULT_RUNAS "celty" CACHE STRING "The default user to run as")
set(DEFAULT_LOCKFILE "${DAEMON_NAME}.lwk" CACHE STRING "The default PID lockfile name")
set(DEFAULT_LOCKDIR "/var/lock/subsys/" CACHE PATH "The default lock directory")
set(DEFAULT_WORKINGDIR "/" CACHE PATH "The default working directory")
set(DEFAULT_MODULEDIR "${CMAKE_INSTALL_PREFIX}/lib/celty" CACHE STRING "The relative path to look for modules in")
set(DEFAULT_MODULEEXT ".moe" CACHE STRING "The default extension for modules")
set(CONFIG_DIRECTORY "${CMAKE_INSTALL_PREFIX}/etc/celty" CACHE STRING "The default configuratio file directory")
set(CONFIG_FILE "celty.cfg" CACHE STRING "The name of the config file")
set(CLIENT_LIST "${CMAKE_INSTALL_PREFIX}/etc/celty/client.def" CACHE STRING "The BitTorrent Client ID list")

set(PID_FILE "/tmp/celty.pid" CACHE STRING "The pid file celty will use for signaling")

configure_file ("${PROJECT_ETC_DIR}/celty-config.hh.in"
                "${PROJECT_BINARY_DIR}/celty-config.hh")

include_directories(${PROJECT_BINARY_DIR})



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fno-builtin")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3 -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-overflow=1 -Wformat=2 -Wuninitialized -Werror=return-type  -Werror=shadow -fstack-protector -Wformat-security")

if(AFL_BUILD)
	find_package(AFL REQUIRED)
	include (CMakeForceCompiler)
	add_definitions(-DAFL_SHIM)
	CMAKE_FORCE_C_COMPILER(${AFL_GCC} AflGCC)
	CMAKE_FORCE_CXX_COMPILER(${AFL_GPP} AflGPP)
endif()

file(GLOB SOURCES "./*.cc")

add_executable(celty ${SOURCES})
target_link_libraries(celty dl ev)

install(TARGETS celty DESTINATION bin)

install(FILES "${PROJECT_ETC_DIR}/celty.cfg" DESTINATION etc/celty)
install(FILES "${PROJECT_ETC_DIR}/client.def" DESTINATION etc/celty)
install(FILES "${PROJECT_ETC_DIR}/celty-proxy.conf" DESTINATION etc/celty)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	install(FILES "${PROJECT_ETC_DIR}/celty.rc.freebsd" DESTINATION /etc/rc.d/ RENAME celty)
endif()

if(SYSTEMD_FOUND)
	install(FILES "${PROJECT_ETC_DIR}/celty.service" DESTINATION ${SYSTEMD_SERVICES_INSTALL_DIR})
endif()

install(FILES "${PROJECT_DOC_DIR}/celty.1" DESTINATION man/man1)


