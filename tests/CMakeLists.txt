find_package(GTest REQUIRED)
find_package(fmt REQUIRED)

# =============== TCP server ===============
add_executable(test_tcp_server
        test_tcp_server.cpp
        ../src/server/tcp_server/tcp_server.cpp
)

target_link_libraries(test_tcp_server PRIVATE
        GTest::gtest
        GTest::gtest_main
        fmt::fmt
)
# =============== TCP server ===============

enable_testing()