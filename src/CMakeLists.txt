set (PI_SOURCES pi_server.c)
set (DTP_SOURCES dtp_server.c)
set (FTP_SOURCES pi_svr_main.c)

add_library (pi_server ${PI_SOURCES})
add_library (dtp_server ${DTP_SOURCES})

add_executable (ftp_svr ${FTP_SOURCES})
#add_dependencies (ftp_svr pi_server)
target_link_libraries(ftp_svr pi_server)

#use C99
set_property(TARGET pi_server dtp_server ftp_svr PROPERTY C_STANDARD 99)

# Enable a few warnings
if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(pi_server PRIVATE -Wall -Wextra)
	target_compile_options(dtp_server PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
	target_compile_options(pi_server PRIVATE /W4)
	target_compile_options(dtp_server PRIVATE /W4)
endif(MSVC)
