set(APP_NAME tcp_server)

find_package(fmt REQUIRED)

add_executable(${APP_NAME}
        main.cpp
        tcp_server.cpp
)

target_link_libraries(${APP_NAME} PRIVATE
        fmt::fmt
)
