set (PI_SOURCES pi_server.c pi_client.c pi_commons.c ftp_commons.c)
set (DTP_SOURCES dtp_server.c dtp_client.c dtp_commons.c ftp_commons.c)
set (FTP_SVR_SOURCES pi_svr_main.c)
set (FTP_CLI_SOURCES pi_cli_main.c)

add_library (ftp_pi ${PI_SOURCES})
add_library (ftp_dtp ${DTP_SOURCES})

add_executable (ftp_svr ${FTP_SVR_SOURCES})
add_executable (ftp_cli ${FTP_CLI_SOURCES})
#add_dependencies (ftp_svr ftp_pi)
target_link_libraries(ftp_svr ftp_pi ftp_dtp)
target_link_libraries(ftp_cli ftp_pi ftp_dtp)

#use C99
set_property(TARGET ftp_pi ftp_dtp ftp_svr ftp_cli PROPERTY C_STANDARD 99)

# Enable a few warnings
if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(ftp_pi PRIVATE -Wall -Wextra)
	target_compile_options(ftp_dtp PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
	target_compile_options(ftp_pi PRIVATE /W4)
	target_compile_options(ftp_dtp PRIVATE /W4)
endif(MSVC)
