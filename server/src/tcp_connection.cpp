#include <server/tcp_connection.h>

namespace Deadpool
{
    TCPConnection::TCPConnection(boost::asio::ip::tcp::socket&& socket) :
        _socket(std::move(socket))
    {
        boost::system::error_code ec;
        std::stringstream addr;

        addr << _socket.remote_endpoint();
        _address = addr.str();
    }

    void TCPConnection::start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler)
    {
        _messageHandler = std::move(messageHandler);
        _errorHandler = std::move(errorHandler);
        asyncRead();
    }

    void TCPConnection::post(const std::string& message)
    {
        bool isQueueEmpty = _outgoingMessages.empty();
        _outgoingMessages.push(message);

        if (isQueueEmpty)
        {
            asyncWrite();
        }
    }

    std::string& TCPConnection::getAddress()
    {
        return _address;
    }

    boost::asio::ip::tcp::socket& TCPConnection::getSocket()
    {
        return _socket;
    }

    void TCPConnection::asyncRead()
    {
        boost::asio::async_read_until(_socket, _streamBuffer, "\n", [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred){
            self->onRead(ec, bytesTransferred);
        });
    }

    void TCPConnection::onRead(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            _socket.close();
            _errorHandler();
            return;
        }

        std::stringstream message;
        message << std::istream(&_streamBuffer).rdbuf();
        _messageHandler(message.str());
        asyncRead();
    }

    void TCPConnection::asyncWrite()
    {
        boost::asio::async_write(_socket, boost::asio::buffer(_outgoingMessages.front()), [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred){
            self->onWrite(ec, bytesTransferred);
        });
    }

    void TCPConnection::onWrite(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            _socket.close();
            _errorHandler();
            return;
        }

        _outgoingMessages.pop();
        if (!_outgoingMessages.empty())
        {
            asyncWrite();
        }
    }
}