#include "tcp_server.hpp"

#include <fmt/printf.h>

#include <boost/asio.hpp>

fast_server::TCP::Server::Server(std::string_view ip, boost::asio::ip::port_type port)
    : m_endpoint{::boost::asio::ip::make_address_v4(ip), port},
      m_ip{ip},
      m_acceptor{std::make_unique<::boost::asio::ip::tcp::acceptor>(m_io_context, m_endpoint)},
      m_port{port},
      m_running{false} {}

fast_server::TCP::Server::~Server() { stop(); }

void fast_server::TCP::Server::run() {
    if (m_running) {
        return;
    }
    m_running = true;

    m_accept_thread = std::thread([this]() {
        while (m_running) {
            ::boost::asio::ip::tcp::socket socket_for_client(m_io_context);
            ::boost::system::error_code ec;
            std::ignore = m_acceptor->accept(socket_for_client, ec);
            if (ec) {
                ::fmt::print(stderr, "Error accepting connection: {}\n", ec.message());
                break;
            }

            m_peers.emplace_back([socket = std::move(socket_for_client), this]() mutable {
                Peer peer{std::move(socket), m_running};
                peer.run();
            });
        }
    });
    ::fmt::print("Server started\n");
}
void fast_server::TCP::Server::stop() {
    if (!m_running) return;

    m_running = false;

    if (m_acceptor && m_acceptor->is_open()) {
        m_acceptor->close();
    }

    unblock_waiting_acceptor();
    m_accept_thread.join();

    for (auto& thread : m_peers) {
        thread.join();
    }

    m_peers.clear();

    ::fmt::print("Server stopped\n");
}
void fast_server::TCP::Server::unblock_waiting_acceptor() const {
    try {
        ::boost::asio::io_context io;
        ::boost::asio::ip::tcp::socket socket(io);
        ::boost::system::error_code ec;
        std::ignore = socket.connect(::boost::asio::ip::tcp::endpoint(::boost::asio::ip::make_address(m_ip), m_port), ec);

        if (!ec) {
            char const* data = "SHUTDOWN";
            boost::asio::write(socket, boost::asio::buffer(data, std::strlen(data)), ec);
        }
    } catch (const std::exception& e) {
        ::fmt::print(stderr, "Error creating unblock connection: {}\n", e.what());
    }
}

fast_server::TCP::Peer::Peer(boost::asio::ip::tcp::socket&& socket, std::atomic<bool>& working_state)
    : m_socket{std::move(socket)}, m_running{working_state} {
    ::fmt::print(stdout, "Peer created. Remote endpoint: {}\n", m_socket.remote_endpoint().address().to_string());
}

void fast_server::TCP::Peer::run() {
    while (m_running) {
        ::boost::system::error_code ec;
        auto const read_bytes = m_socket.read_some(::boost::asio::buffer(m_buffer.data(), 1024), ec);
        if (ec) {
            ::fmt::print(stderr, "Error reading data: {}\n", ec.message());
            break;
        }
        m_buffer[read_bytes] = '\0';
        ::fmt::print(stdout, "Received: {}\n", m_buffer.data());
    }
    ::fmt::print(stdout, "Peer stopped. Remote endpoint: {}\n", m_socket.remote_endpoint().address().to_string());
}
void fast_server::TCP::Peer::stop() {
    m_running = false;

    if (m_socket.is_open()) {
        boost::system::error_code ec;
        std::ignore = m_socket.close(ec);
        if (ec) {
            ::fmt::print(stderr, "Error closing socket: {}\n", ec.message());
        }
    }
}