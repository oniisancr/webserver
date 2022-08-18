cmake_minimum_required(VERSION 3.0.0)
project(webserver VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()
include_directories(/usr/local/include sockets/header sockets/src servers/header servers/src web/header web/src)

add_executable(webserver main.cpp servers/src/simplepoller.cpp web/src/simpleweb.cpp servers/src/webserver.cpp servers/src/simpleserver.cpp sockets/src/simplesocket.cpp
    sockets/src/bindingsocket.cpp sockets/src/listeningsocket.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring on/for Linux")
    add_library(poller
        STATIC
        servers/header/epoller.hpp
        servers/src/epoller.cpp
    )
    target_compile_definitions(webserver PUBLIC "IS_LINUX")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring on/for macOS")
    add_library(poller
        STATIC
        servers/header/kqueuepoller.hpp
        servers/src/kqueuepoller.cpp
    )
    target_compile_definitions(webserver PUBLIC "IS_MACOS")
else()
    message(FATAL_ERROR "Can not be intalled on this Machine")
endif()

# 导入库
target_link_libraries(webserver poller)

file(COPY webcontent DESTINATION ${CMAKE_BINARY_DIR})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
