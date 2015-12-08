find_program(AFL_CLANG afl-clang
	/usr/bin
	/usr/local/bin
	/bin
)
find_program(AFL_GCC afl-gcc
	/usr/bin
	/usr/local/bin
	/bin
)
find_program(AFL_GPP afl-g++
	/usr/bin
	/usr/local/bin
	/bin
)
mark_as_advanced(AFL_CLANG AFL_GCC AFL_GPP)
