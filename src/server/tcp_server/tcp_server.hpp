#pragma once
#include <string_view>

namespace fast_server::TCP {
class Server {
   public:
    Server(std::string_view ip, uint32_t port);
    ~Server() = default;

    Server(Server const&) = delete;
    Server& operator=(Server const&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    void run();
    void stop();
   private:
    std::string_view m_ip; // 16
    uint32_t m_port;       // 4
};
} // namespace fast_server::TCP