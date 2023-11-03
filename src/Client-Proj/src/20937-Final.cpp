#include "Client.h"

int main()
{
    char recvbuf[1024] = {0};
    const char* sendbuf = R"(Hello world!)";
    auto c = Client();
    //c.send(sendbuf, strlen(sendbuf) + 1);
}