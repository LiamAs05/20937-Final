#include "Client.h"

int main()
{
    char recvbuf[1024] = {0};
    const char* sendbuf = R"(Hello world!)";
    auto c = Client();
    c.send(sendbuf, 14);
    c.recv(recvbuf, 1024);
    std::cout << recvbuf << std::endl;
}