# Returns the short git revision string from the current head
# Borrowed from https://github.com/google/benchmark/blob/master/cmake/GetGitVersion.cmake
find_package(Git)

if(__get_git_ver)
	return()
endif()
set(__get_git_ver INCLUDE)

function(get_git_version arg1)
	if(GIT_EXECUTABLE)
		execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
			RESULT_VARIABLE status
        	OUTPUT_VARIABLE GIT_VERSION
        	ERROR_QUIET)
		if(${status})
			set(GIT_VERSION "0000000")
		else()
			string(STRIP ${GIT_VERSION} GIT_VERSION)
		endif()

		execute_process(COMMAND ${GIT_EXECUTABLE} update-index -q --refresh
			OUTPUT_QUIET
			ERROR_QUIET)
		execute_process(COMMAND ${GIT_EXECUTABLE} diff-index --name-only HEAD --
        	OUTPUT_VARIABLE GIT_DIFF_INDEX
        	ERROR_QUIET)
	    string(COMPARE NOTEQUAL "${GIT_DIFF_INDEX}" "" GIT_DIRTY)
	    if (${GIT_DIRTY})
			set(GIT_VERSION "${GIT_VERSION}-dirty")
    	endif()
	else()
		set(GIT_VERSION "0000000")
	endif()
	message(STATUS "-- git version: ${GIT_VERSION}")
	set(${arg1} ${GIT_VERSION} PARENT_SCOPE)
endfunction()
