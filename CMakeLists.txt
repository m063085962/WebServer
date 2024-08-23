cmake_minimum_required (VERSION 3.1)

project (Server
		 VERSION 0.1
		 LANGUAGES C CXX
		)

include_directories (${PROJECT_SOURCE_DIR}/include)
aux_source_directory (${PROJECT_SOURCE_DIR}/src SRCLIST)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set (CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_STANDARD 17)
add_compile_options (-Wall -g)

add_subdirectory (test)

find_package (Threads REQUIRED)
add_executable (server server.cpp ${SRCLIST})
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})

