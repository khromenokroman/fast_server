#pragma once
#include <boost/asio.hpp>
#include <list>
#include <string_view>

namespace fast_server::TCP {

class Peer {
   public:
    explicit Peer(::boost::asio::ip::tcp::socket&& socket, std::atomic<bool>& working_state);

    Peer(Peer const&) = delete;
    Peer& operator=(Peer const&) = delete;
    Peer(Peer&&) = delete;
    Peer& operator=(Peer&&) = delete;

    void run();
    void stop();

   private:
    std::array<char, 4096> m_buffer{};       // 4096
    ::boost::asio::ip::tcp::socket m_socket; // 88
    std::atomic<bool>& m_running;            // 8
};

class Server {
   public:
    explicit Server(std::string_view ip, boost::asio::ip::port_type port);
    ~Server();

    Server(Server const&) = delete;
    Server& operator=(Server const&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    void run();
    void stop();

   private:
    void unblock_waiting_acceptor() const;
    ::boost::asio::ip::tcp::endpoint m_endpoint;                  // 28
    std::list<std::thread> m_peers;                               // 24
    ::boost::asio::io_context m_io_context;                       // 16
    std::string_view m_ip;                                        // 16
    std::unique_ptr<::boost::asio::ip::tcp::acceptor> m_acceptor; // 8
    ::boost::asio::ip::port_type m_port;                          // 8
    std::thread m_accept_thread;                                  // 8
    std::atomic<bool> m_running;                                  // 1
};

} // namespace fast_server::TCP