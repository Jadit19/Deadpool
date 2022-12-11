#include <server/tcp_server.h>

int main(int argc, char** argv)
{
    Deadpool::TCPServer server(Deadpool::IPV::v4, 3000);

    server.run();

    return EXIT_SUCCESS;
}