cmake_minimum_required (VERSION 3.1)

project (WebServer
		 VERSION 0.1
		 LANGUAGES C CXX
		)

include_directories (${PROJECT_SOURCE_DIR}/include)
set (ECHO ${PROJECT_SOURCE_DIR}/src/echoserver.cpp)
set (TEST ${PROJECT_SOURCE_DIR}/test/test.cpp)
aux_source_directory (${PROJECT_SOURCE_DIR}/src/base BASE)
aux_source_directory (${PROJECT_SOURCE_DIR}/src/threads THREAD)
aux_source_directory (${PROJECT_SOURCE_DIR}/src/http HTTP)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set (CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_STANDARD 17)
add_compile_options (-Wall -g)


find_package (Threads REQUIRED)
add_executable (server ${ECHO} ${BASE} ${THREAD} ${HTTP})
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})

