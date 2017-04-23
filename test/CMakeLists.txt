add_executable(pi_test pi_test.c)
add_executable(dtp_test dtp_test.c)

target_link_libraries(pi_test ftp_pi ${CRITERION_LIBRARIES})
target_link_libraries(dtp_test ftp_dtp ${CRITERION_LIBRARIES})

#use C99
set_property(TARGET pi_test PROPERTY C_STANDARD 99)
set_property(TARGET dtp_test PROPERTY C_STANDARD 99)

# Enable a few warnings
if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(pi_test PRIVATE -Wall -Wextra)
	target_compile_options(dtp_test PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
	target_compile_options(pi_test PRIVATE /W4)
	target_compile_options(dtp_test PRIVATE /W4)
endif(MSVC)

add_test (pi_test pi_test)
add_test (dtp_test dtp_test)