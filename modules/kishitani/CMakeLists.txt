cmake_minimum_required(VERSION 2.8.0)
project(Celty)
option(BUILD_KISHITANI_MODULE "Build the Celty Kishitani Module" ON)



if(BUILD_KISHITANI_MODULE)
	find_package(LIBPQXX REQUIRED)

	if(PQXX_FOUND)
		include_directories(${CMAKE_SOURCE_DIR}/src/include)
		include_directories(${PROJECT_BINARY_DIR})
		include_directories(${PQXX_INCLUDE_DIR})

		if (CMAKE_BUILD_TYPE STREQUAL "Debug")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fno-builtin")
		endif()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3 -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-overflow=1 -Wformat=2 -Wuninitialized -Werror=return-type  -Werror=shadow -fstack-protector -Wformat-security")

		file(GLOB SOURCES "./*.cc")


		add_library(kishitani SHARED ${SOURCES})
		target_link_libraries(kishitani ${PQXX_LIBRARIES})
		set_target_properties(kishitani PROPERTIES PREFIX "")
		set_target_properties(kishitani PROPERTIES SUFFIX ${DEFAULT_MODULEEXT})
	else()
		message(FATAL_ERROR "Unable to find libpqxx, can't build kishitani connector")
	endif()
endif()
