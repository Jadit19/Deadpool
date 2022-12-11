#include <thread>

#include <client/tcp_client.h>

int main (int argc, char** argv)
{
    Deadpool::TCPClient client("localhost", 3000);

    std::thread thread { [&client](){
        client.run();
    }};

    while (true)
    {
        std::string message;
        getline(std::cin, message);
        if (message == "\\q")
        {
            break;
        }
        else if (message == "\\sq")
        {
            client.saveChat("/home/adit/");
            break;
        }

        message += "\n";
        client.post(message);
    }

    client.stop();
    thread.join();

    return EXIT_SUCCESS;

}