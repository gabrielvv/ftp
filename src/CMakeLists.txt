set (PI_SOURCES
	pi_server.c
)
#set (DTP_SOURCES dtp_server.c)

add_library (pi_server ${PI_SOURCES})
#add_library (dtp_server ${DTP_SOURCES})

#use C99
set_property(TARGET pi_server PROPERTY C_STANDARD 99)

# Enable a few warnings
if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(pi_server PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
	target_compile_options(pi_server PRIVATE /W4)
endif(MSVC)
