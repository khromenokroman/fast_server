cmake_minimum_required(VERSION 3.16)
set(APP_NAME tcp_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(${APP_NAME}
        main.cpp
        tcp_server.cpp
)
