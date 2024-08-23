file (GLOB SRCLIST_C ${PROJECT_SOURCE_DIR}/src/*.cpp)
list (REMOVE_ITEM SRCLIST_C ${PROJECT_SOURCE_DIR}/src/ThreadPool.cpp)
list (REMOVE_ITEM SRCLIST_C ${PROJECT_SOURCE_DIR}/src/TcpServer.cpp)

add_executable (client client.cpp ${SRCLIST_C})

