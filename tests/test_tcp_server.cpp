#include <fmt/format.h>
#include <gtest/gtest.h>

#include <boost/asio.hpp>

#include "../src/server/tcp_server/tcp_server.hpp"

static constexpr char const* const DEFAULT_IP = "127.0.0.1";
static constexpr unsigned short int const DEFAULT_PORT = 6001;

TEST(FastServer, createTCPServer) { ASSERT_NO_THROW(::fast_server::TCP::Server(DEFAULT_IP, DEFAULT_PORT)); }

TEST(FastServer, runStopServer) {
    ::fast_server::TCP::Server server(DEFAULT_IP, DEFAULT_PORT);
    ASSERT_NO_THROW(server.run());
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

TEST(FastServer, send_one_message) {
    ::fast_server::TCP::Server server(DEFAULT_IP, DEFAULT_PORT);
    server.run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    ::boost::asio::io_context io;
    ::boost::asio::ip::tcp::socket socket(io);
    ::boost::system::error_code ec;
    socket.connect(::boost::asio::ip::tcp::endpoint(::boost::asio::ip::make_address(DEFAULT_IP), DEFAULT_PORT), ec);
    ASSERT_FALSE(ec) << ::fmt::format("Error connecting to server '{}:{}': {}", DEFAULT_IP, DEFAULT_PORT, ec.message());

    char const* data = "TEST MESSAGE";
    auto write_bytes = boost::asio::write(socket, boost::asio::buffer(data, std::strlen(data)), ec);
    ASSERT_FALSE(ec) << ::fmt::format("Error writing to server '{}:{}': {}", DEFAULT_IP, DEFAULT_PORT, ec.message());
    ASSERT_EQ(write_bytes, std::strlen(data));

    ASSERT_NO_THROW(server.stop());
}
TEST(FastServer, send_500000_message) {
    ::fast_server::TCP::Server server(DEFAULT_IP, DEFAULT_PORT);
    server.run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    ::boost::asio::io_context io;
    ::boost::asio::ip::tcp::socket socket(io);
    ::boost::system::error_code ec;
    socket.connect(::boost::asio::ip::tcp::endpoint(::boost::asio::ip::make_address(DEFAULT_IP), DEFAULT_PORT), ec);
    ASSERT_FALSE(ec) << ::fmt::format("Error connecting to server '{}:{}': {}", DEFAULT_IP, DEFAULT_PORT, ec.message());

    char const* data = "TEST MESSAGE\n";
    const size_t data_len = std::strlen(data);

    auto start_time = std::chrono::high_resolution_clock::now();
    // Отправляем 500 000 сообщений
    for (int i = 0; i < 500000; ++i) {
        auto write_bytes = boost::asio::write(socket, boost::asio::buffer(data, data_len), ec);
        ASSERT_FALSE(ec) << ::fmt::format("Error writing to server '{}:{}': {}", DEFAULT_IP, DEFAULT_PORT, ec.message());
        ASSERT_EQ(write_bytes, data_len);
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    ::fmt::print(stdout, "Duration: {}ms\n", duration.count());

    ASSERT_NO_THROW(server.stop());
}

TEST(FastServer, send_500000_message_batch) {
    ::fast_server::TCP::Server server(DEFAULT_IP, DEFAULT_PORT);
    server.run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    ::boost::asio::io_context io;
    ::boost::asio::ip::tcp::socket socket(io);
    ::boost::system::error_code ec;
    socket.connect(::boost::asio::ip::tcp::endpoint(::boost::asio::ip::make_address(DEFAULT_IP), DEFAULT_PORT), ec);
    ASSERT_FALSE(ec) << ::fmt::format("Error connecting to server '{}:{}': {}", DEFAULT_IP, DEFAULT_PORT, ec.message());

    char const* data = "TEST MESSAGE\n";
    const size_t data_len = std::strlen(data);

    // пачку создадим 1000 шт.
    const size_t messages_per_batch = 1000;
    const size_t batch_size = data_len * messages_per_batch;
    std::vector<char> batch(batch_size);
    for (size_t i = 0; i < messages_per_batch; ++i) {
        std::memcpy(batch.data() + i * data_len, data, data_len);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    // Отправляем 500 000 сообщений
    for (int i = 0; i < 500; ++i) {
        auto write_bytes = boost::asio::write(socket, boost::asio::buffer(batch, batch_size), ec);
        ASSERT_FALSE(ec) << ::fmt::format("Error writing to server '{}:{}': {}", DEFAULT_IP, DEFAULT_PORT, ec.message());
        ASSERT_EQ(write_bytes, batch_size);
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    ::fmt::print(stdout, "Duration batch: {}ms\n", duration.count());

    ASSERT_NO_THROW(server.stop());
}