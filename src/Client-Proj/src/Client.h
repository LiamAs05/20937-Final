#pragma once
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Utils.h"

#define REPEAT_THREE_TIMES(code) \
    for (int i = 0; i < 3; i++) { \
        code \
    }

#define ME_INFO_EXISTS 1
#define ME_INFO_MISSING 0

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
	// internal client utility functions
	void get_transfer_info();
	bool get_me_info();
	static void startup();
	void resolveAddress();
	void connect();

	// internal client data
	std::string ip;
	u_short port;
	std::string name;
	std::string path;
	std::string unique_id;
	std::string private_key;
	SOCKET ConnectSocket;
	struct sockaddr_in clientService;
};

