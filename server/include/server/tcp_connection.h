#pragma once

#include <iostream>
#include <memory>
#include <queue>

#include <boost/asio.hpp>

namespace Deadpool
{
    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

    class TCPConnection : public std::enable_shared_from_this<TCPConnection>
    {
        private:
            std::string _address;
            std::queue<std::string> _outgoingMessages;

            boost::asio::streambuf _streamBuffer { 65536 };
            boost::asio::ip::tcp::socket _socket;

            MessageHandler _messageHandler;
            ErrorHandler _errorHandler;

            void asyncRead();
            void onRead(boost::system::error_code ec, size_t bytesTransferred);

            void asyncWrite();
            void onWrite(boost::system::error_code ec, size_t bytesTransferred);

        public:
            using pointer = std::shared_ptr<TCPConnection>;

            static pointer create(boost::asio::ip::tcp::socket&& socket)
            {
                return pointer(new TCPConnection(std::move(socket)));
            }
            explicit TCPConnection(boost::asio::ip::tcp::socket&& socket);

            void start(MessageHandler&& messagehandler, ErrorHandler&& errorHandler);
            void post(const std::string& message);
            
            std::string& getAddress();

            boost::asio::ip::tcp::socket& getSocket();
    };
}