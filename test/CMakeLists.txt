add_executable(server_test test.c)

target_link_libraries(server_test server ${CRITERION_LIBRARIES})

#use C99
set_property(TARGET server_test PROPERTY C_STANDARD 99)

# Enable a few warnings
if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(server PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
	target_compile_options(server PRIVATE /W4)
endif(MSVC)

add_test (server_test server_test)
