#include <fmt/format.h>
#include <gtest/gtest.h>

#include <boost/asio.hpp>

#include "../src/server/tcp_server/tcp_server.hpp"

static constexpr char const* const DEFAULT_IP = "127.0.0.1";
static constexpr unsigned short int const DEFAULT_PORT = 6001;

TEST(FastServer, createTCPServer) { ASSERT_NO_THROW(::fast_server::TCP::Server(DEFAULT_IP, DEFAULT_PORT)); }

TEST(FastServer, runServer) { ASSERT_NO_THROW(::fast_server::TCP::Server(DEFAULT_IP, DEFAULT_PORT).run()); }

TEST(FastServer, stopServer) {
    ::fast_server::TCP::Server server(DEFAULT_IP, DEFAULT_PORT);
    server.run();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_NO_THROW(server.stop());
}

TEST(FastServer, connect2Server) {
    ::fast_server::TCP::Server server(DEFAULT_IP, DEFAULT_PORT);
    server.run();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ::boost::asio::io_context io_context;
    ::boost::asio::ip::tcp::endpoint endpoint{::boost::asio::ip::make_address(DEFAULT_IP), DEFAULT_PORT};
    std::unique_ptr<::boost::asio::ip::tcp::socket> socket = std::make_unique<::boost::asio::ip::tcp::socket>(io_context);

    boost::system::error_code ec;
    std::ignore = socket->connect(endpoint, ec);
    ASSERT_FALSE(ec) << ::fmt::format("Error connecting to server '{}:{}': {}", DEFAULT_IP, DEFAULT_PORT, ec.message());
    ASSERT_NO_THROW(server.stop());
}