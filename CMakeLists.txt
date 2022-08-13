cmake_minimum_required(VERSION 3.0.0)
project(webserver VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()
include_directories(sockets/header sockets/src servers/header servers/src)

add_executable(webserver main.cpp servers/src/testserver.cpp servers/src/simpleserver.cpp sockets/src/simplesocket.cpp
sockets/src/bindingsocket.cpp sockets/src/listeningsocket.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
