set (SOURCES
	server.c
)

add_library (server ${SOURCES})

#use C99
set_property(TARGET server PROPERTY C_STANDARD 99)

# Enable a few warnings
if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(server PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
	target_compile_options(server PRIVATE /W4)
endif(MSVC)
