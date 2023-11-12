#pragma once
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Utils.h"
#include "ResponseParser.h"
#include "RequestBuilder.h"
#include "RSAWrapper.h"

/**
 * \brief Ever wanted something repeated 3 times?
 * \param code Code block to repeat
 */
#define REPEAT_THREE_TIMES(code) \
    for (int i = 0; i < 3; i++) { \
        code \
    }

/**
 * \brief Readable booleans that check if `me.info` exists
 */
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
	void send(const char* buf, unsigned len) const;
	void recv(char* buf, unsigned len) const;
	~Client() = default;

	std::string get_name();
	void set_name(const std::string& name);

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
	RequestBuilder req_builder;
	ResponseParser res_parser;
	RSAPrivateWrapper private_key_wrapper;
	struct sockaddr_in clientService;
};

