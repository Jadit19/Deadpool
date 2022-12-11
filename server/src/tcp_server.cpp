#include <server/tcp_server.h>

namespace Deadpool
{
    TCPServer::TCPServer(IPV ipv, int port) :
        _ipv(ipv),
        _port(port),
        _acceptor(boost::asio::ip::tcp::acceptor(_ioContext, boost::asio::ip::tcp::endpoint(_ipv == IPV::v4 ?
            boost::asio::ip::tcp::v4() :
            boost::asio::ip::tcp::v6()
        , _port)))
    {
        
    }

    int TCPServer::run()
    {
        try
        {
            startAccepting();
            std::cout << "Running server on port #" << _port << "..." << std::endl;
            _ioContext.run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Error!" << std::endl << e.what() << std::endl;
            return -1;
        }
        return 0;
    }

    void TCPServer::broadcast(const std::string& message)
    {
        for (const TCPConnection::pointer& connection: _connections)
        {
            connection->post(message);
        }
    }

    void TCPServer::startAccepting()
    {
        _socket.emplace(_ioContext);
        _acceptor.async_accept(*_socket, [this](const boost::system::error_code& error){
            std::shared_ptr<TCPConnection> connection = TCPConnection::create(std::move(*_socket));

            std::cout << connection->getAddress() << " has joined the party!" << std::endl;

            this->_connections.insert(connection);
            if (!error)
            {
                connection->start(
                    [this](const std::string& message)
                    {
                        broadcast(message);
                    },
                    [&, weak = std::weak_ptr(connection)]()
                    {
                        if (auto shared = weak.lock(); shared && this->_connections.erase(shared))
                        {
                            std::cout << shared->getAddress() << " has left the chat..." << std::endl;
                        }
                    }
                );
            }
            
            this->startAccepting();
        });
    }
}