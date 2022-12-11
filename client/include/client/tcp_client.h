#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <boost/asio.hpp>

#include <critical/message.h>

namespace Deadpool
{
    class TCPClient
    {
        private:
            std::string _username;
            std::queue<std::string> _outgoingMessages;
            std::vector<std::string> _chatHistory;

            boost::asio::io_context _ioContext;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::ip::tcp::resolver::results_type _endpoints;
            boost::asio::streambuf _streamBuffer { 65536 };

            void asyncRead();
            void onRead(boost::system::error_code ec, size_t bytesTransferred);

            void asyncWrite();
            void onWrite(boost::system::error_code ec, size_t bytesTransferred);

        public:
            TCPClient(const std::string& address, int port);

            void run();
            void stop();
            void post(const std::string& message);

            void handleMessage(const std::string& message);
            void saveChat(const std::string& fileDirectoryPath);
    };
}