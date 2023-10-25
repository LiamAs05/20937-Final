#pragma once
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#define REPEAT_THREE_TIMES(code) \
    for (int i = 0; i < 3; i++) { \
        code \
    }

enum
{
	ERR = -1
};

class Client
{
public:
	Client();
	void send(const char* buf, int len) const;
	void recv(char* buf, int len) const;
	~Client() = default;

private:
	void startup();
	void resolveAddress();
	void connect();
	SOCKET ConnectSocket;
	struct sockaddr_in clientService;
};
