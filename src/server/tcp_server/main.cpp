#include <fmt/printf.h>

#include "tcp_server.hpp"
int main() {
    try {
        ::fast_server::TCP::Server server("127.0.0.1", 6001);

        server.run();

        std::this_thread::sleep_for(std::chrono::seconds(10));
        ::fmt::print("Timeout\n");

        server.stop();

        return 0;
    } catch (std::exception const& ex) {
        ::fmt::print(stderr, "Ex: {}\n", ex.what());
        return -1;
    }
}