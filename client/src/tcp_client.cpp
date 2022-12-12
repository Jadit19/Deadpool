#include <client/tcp_client.h>

namespace Deadpool
{
    TCPClient::TCPClient(const std::string& address, int port) :
        _socket(_ioContext)
    {
        boost::asio::ip::tcp::resolver resolver { _ioContext };
        _endpoints = resolver.resolve(address, std::to_string(port));

        system("clear");

        std::cout << "> What do we call you?: ";
        getline(std::cin, _username);
        std::cout << "> " << _username << " has joined the party!" << std::endl << std::endl;
    }

    void TCPClient::run()
    {
        boost::asio::async_connect(_socket, _endpoints, [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ep){
            if (!ec)
            {
                asyncRead();
            }
        });

        _ioContext.run();
    }

    void TCPClient::stop()
    {
        boost::system::error_code ec;
        _socket.close(ec);
    }

    void TCPClient::post(const std::string& message)
    {
        bool isQueueIdle = _outgoingMessages.empty();

        time_t currentTime;
        time(&currentTime);

        Message newMessage(_username, message, currentTime);
        _outgoingMessages.push(newMessage.getJson());

        if (isQueueIdle)
        {
            asyncWrite();
        }
    }

    void TCPClient::handleMessage(const std::string& message)
    {
        Message receivedMsg(message);
        if (receivedMsg.getSender() == _username)
        {
            printf("\x1b[1F");
            printf("\x1b[2K");
        }

        std::cout << "> " << receivedMsg.getSender();
        int senderNameSize = 14 - receivedMsg.getSender().size();
        
        while (senderNameSize > 0)
        {
            std::cout << "\t";
            senderNameSize -= 8;
        }

        std::cout << ": " << receivedMsg.getData() << std::endl;
        _chatHistory.push_back(receivedMsg.getJson());
    }

    void TCPClient::saveChat(const std::string& fileDirectoryPath)
    {
        std::ofstream chatFile(fileDirectoryPath + _username + ".json");
        chatFile << "[\n";

        int size = _chatHistory.size();
        for (int i=0; i<size-1; i++)
        {
            chatFile << "\t" << _chatHistory[i] << ",\n";
        }
        chatFile << "\t" << _chatHistory[size-1] << "\n]";

        chatFile.close();
    }

    void TCPClient::asyncRead()
    {
        boost::asio::async_read_until(_socket, _streamBuffer, "\n", [this](boost::system::error_code ec, size_t bytesTransferred){
            onRead(ec, bytesTransferred);
        });
    }
    
    void TCPClient::onRead(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            stop();
            return;
        }

        std::stringstream message;
        message << std::istream(&_streamBuffer).rdbuf();
        handleMessage(message.str());
        asyncRead();
    }

    void TCPClient::asyncWrite()
    {
        boost::asio::async_write(_socket, boost::asio::buffer(_outgoingMessages.front()), [this](boost::system::error_code ec, size_t bytesTransferred){
            onWrite(ec, bytesTransferred);
        });
    }

    void TCPClient::onWrite(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            stop();
            return;
        }

        _outgoingMessages.pop();
        if (!_outgoingMessages.empty())
        {
            asyncWrite();
        }
    }
}