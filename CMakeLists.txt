cmake_minimum_required(VERSION 3.16)
project(fast_server)

set(CMAKE_CXX_STANDARD 20)

add_link_options(
        -Wl,--no-undefined
)

#add_executable(fast_server main.cpp)

add_subdirectory(src)
add_subdirectory(tests)
