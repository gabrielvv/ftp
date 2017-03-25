# Copy of
# https://github.com/Snaipe/Criterion/blob/bleeding/dev/cmake/CMakeLists.txt
#

cmake_minimum_required (VERSION 2.8.9)

#set(CMAKE_BUILD_TYPE Release)

project (ftp C)

# Add ./cmake to the module path
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option (ENABLE_TESTS "Build test along with the library" ON)

# Add interface to include dirs
#include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src") 

# Add sources
add_subdirectory (src)
add_subdirectory (doc)
add_subdirectory (examples)

if (ENABLE_TESTS)
	# Check for criterion
	find_package (Criterion REQUIRED)

	# Add tests
	enable_testing ()
	add_subdirectory (test)
endif ()
