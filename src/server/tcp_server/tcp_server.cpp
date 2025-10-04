#include "tcp_server.hpp"

#include <boost/asio.hpp>

// void ServerTCP() {
//     ::boost::asio::io_context io_context;
//     ::boost::asio::ip::tcp::socket peer_socket(io_context);
//     ::boost::asio::ip::tcp::endpoint endpoint = {::boost::asio::ip::make_address_v4("0.0.0.0"), 6001};
//
//     ::boost::asio::ip::tcp::acceptor acceptor(io_context, endpoint);
//
//     std::unique_ptr<char[]> data_recv(new char[1024]);
//     std::atomic<std::size_t> count_messages = 0;
//
//     std::function<void(::boost::system::error_code ec, std::size_t bytes_recvd)> reciver;
//     reciver = [&](::boost::system::error_code ec, std::size_t bytes_recvd) {
//         if (!ec) {
//             ++count_messages;
//             // std::cout << "Message: " << std::string_view(data_recv.get(), bytes_recvd) << "count: " << count_messages
//             // << std::endl;
//             peer_socket.async_receive(::boost::asio::buffer(data_recv.get(), 1024), reciver);
//         } else {
//             std::cout << "Error: " << ec.message() << std::endl;
//         }
//     };
//
//     acceptor.async_accept(peer_socket, [&](::boost::system::error_code ec) {
//         if (!ec) {
//             std::cout << "Client connected" << std::endl;
//             std::cout << "Client address: " << peer_socket.remote_endpoint().address().to_string() << std::endl;
//             std::cout << "Client port: " << peer_socket.remote_endpoint().port() << std::endl;
//             peer_socket.async_receive(::boost::asio::buffer(data_recv.get(), 1024), reciver);
//         }
//     });
//
//     io_context.run();
//     std::cout << "Messages: " << count_messages << std::endl;
// }
fast_server::TCP::Server::Server(std::string_view ip, uint32_t port) : m_ip{ip}, m_port{port} {}
void fast_server::TCP::Server::run() {
    // ::boost::asio::io_context io_context;
    // ::boost::asio::ip::tcp::socket peer_socket(io_context);
    // ::boost::asio::ip::tcp::endpoint endpoint = {::boost::asio::ip::make_address_v4("0.0.0.0"), 6001};
    //
    // ::boost::asio::ip::tcp::acceptor acceptor(io_context, endpoint);
    //
    // std::unique_ptr<char[]> data_recv(new char[1024]);
    // std::atomic<std::size_t> count_messages = 0;
    //
    // std::function<void(::boost::system::error_code ec, std::size_t bytes_recvd)> reciver;
    // reciver = [&](::boost::system::error_code ec, std::size_t bytes_recvd) {
    //     if (!ec) {
    //         ++count_messages;
    //         // std::cout << "Message: " << std::string_view(data_recv.get(), bytes_recvd) << "count: " << count_messages
    //         // << std::endl;
    //         peer_socket.async_receive(::boost::asio::buffer(data_recv.get(), 1024), reciver);
    //     } else {
    //         // std::cout << "Error: " << ec.message() << std::endl;
    //     }
    // };
    //
    // acceptor.async_accept(peer_socket, [&](::boost::system::error_code ec) {
    //     if (!ec) {
    //         // std::cout << "Client connected" << std::endl;
    //         // std::cout << "Client address: " << peer_socket.remote_endpoint().address().to_string() << std::endl;
    //         // std::cout << "Client port: " << peer_socket.remote_endpoint().port() << std::endl;
    //         peer_socket.async_receive(::boost::asio::buffer(data_recv.get(), 1024), reciver);
    //     }
    // });
    //
    // io_context.run();
    // // std::cout << "Messages: " << count_messages << std::endl;
}
void fast_server::TCP::Server::stop() {}