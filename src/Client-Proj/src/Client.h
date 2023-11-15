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
#include "AESWrapper.h"
#include "Base64Wrapper.h"

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
	me_info_missing = false,
	me_info_exists = true
};

class Client
{
public:
	Client();
	void send(const char* buf, unsigned long long len);
	void recv(char* buf, unsigned long long len);
	~Client();

	std::string get_name();
	void set_name(const std::string& name);
	std::string get_id();
	void set_id(const std::string& id);
	void set_id(std::array<char, size_req_client_id>& id);
	void set_aes_key(const std::vector<unsigned char>& new_key);
	std::vector<unsigned char> get_aes_key();
	unsigned int crc = 0;

private:
	// internal client utility functions
	void send_invalid_crc();
	void send_final_invalid_crc();
	void send_valid_crc();
	bool send_file();
	void send_public_key();
	void establish_server_connectivity();
	void get_transfer_info();
	bool get_me_info();
	void create_me_info() const;
	void register_as_new_client();
	void login_as_existing_client();
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
	std::vector<unsigned char> aes_key;
	SOCKET ConnectSocket;
	RequestBuilder req_builder;
	ResponseParser res_parser;
	RSAPrivateWrapper* private_key_wrapper;
	AESWrapper* aes_wrapper;
	struct sockaddr_in clientService;
	bool new_user = true;
};

