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

enum
{
	err = -1,
	me_info_missing = true
};

class

Client
{
public:
	Client();
	void send(const char* buf, unsigned len) const;
	void recv(char* buf, unsigned len) const;
	~Client() = default;

	std::string get_name();
	void set_name(const std::string& name);
	std::string get_id();
	void set_id(const std::string& id);
	void set_id(std::array<char, size_req_client_id>& id);

private:
	// internal client utility functions
	void establish_server_connectivity();
	void get_transfer_info();
	bool get_me_info();
	void register_as_new_client();
	static void startup();
	void resolveAddress();
	void connect();

	// internal client data
	std::string ip;
	u_short port;
	std::string name;
	std::string path;
	std::string unique_id_str;
	std::array<char, size_req_client_id> unique_id_bytes;
	std::string private_key;
	SOCKET ConnectSocket;
	RequestBuilder req_builder;
	ResponseParser res_parser;
	RSAPrivateWrapper private_key_wrapper;
	struct sockaddr_in clientService;
};

