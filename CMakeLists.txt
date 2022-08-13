cmake_minimum_required(VERSION 3.0.0)
project(webserver VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

include(CTest )
enable_testing()
include_directories(/usr/local/include sockets/header sockets/src servers/header servers/src web/header web/src)

add_executable(webserver main.cpp  web/src/simpleweb.cpp servers/src/webserver.cpp servers/src/simpleserver.cpp sockets/src/simplesocket.cpp
sockets/src/bindingsocket.cpp sockets/src/listeningsocket.cpp)

file(COPY webcontent DESTINATION ${CMAKE_BINARY_DIR})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
