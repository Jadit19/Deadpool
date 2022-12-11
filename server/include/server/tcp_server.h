#pragma once

#include <functional>
#include <optional>
#include <unordered_set>

#include <critical/ipv.h>
#include <server/tcp_connection.h>

namespace Deadpool
{
    class TCPServer
    {
        private:
            IPV _ipv;
            int _port;

            boost::asio::io_context _ioContext;
            boost::asio::ip::tcp::acceptor _acceptor;

            std::unordered_set<TCPConnection::pointer> _connections;
            std::optional<boost::asio::ip::tcp::socket> _socket;

            void startAccepting();
        
        public:
            TCPServer(IPV ipv, int port);

            int run();
            void broadcast(const std::string& message);
    };
}