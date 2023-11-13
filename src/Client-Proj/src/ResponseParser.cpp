#include "ResponseParser.h"
#include "Client.h"

ResponseParser::ResponseParser(Client* a)
{
	client = a;
}

bool ResponseParser::parse_response(std::vector<char>& response)
{
	if (response.size() < size_code + size_payload_size + size_version)
	{
		std::cerr << "An unknown error occurred, invalid response from server." << std::endl;
		_exit(UNKNOWN_ERROR);
	}

	const ResponseCodes code = get_code({ response[1], response[2] });
	const unsigned payload_size = bytes_to_int({ response[3], response[4], response[5], response[6] });

	return parse_code(code, response, payload_size);
}

bool ResponseParser::parse_code(ResponseCodes code, std::vector<char>& response, unsigned payload_size)
{
	// ReSharper disable once CppLocalVariableMayBeConst
	std::vector<char> payload(payload_size);
	std::copy_n(response.begin() + size_headers, payload_size, payload.begin());
	std::array<char, size_req_client_id> uid;

	switch (code)
	{
	case register_success:
		client->recv(uid.data(), size_req_client_id);
		client->set_id(uid);
		std::cout << "Client ID: " << client->get_id();
		return true;
	case register_fail:
		std::cerr << "Registration has failed, restart as a new client..." << std::endl;
		return false;
	case public_key_success:
		return true;
	case valid_crc:
		return true;
	case message_success:
		client->recv(uid.data(), size_req_client_id);
		client->set_id(uid);
		std::cout << "This concludes our session, Goodbye client " << client->get_id();
		return false;
	case login_success:
		return true;
	case login_fail:
		client->recv(uid.data(), size_req_client_id);
		client->set_id(uid);
		std::cerr << "Login has failed, this can happen either when\n\
1. The username already exists in the DB\n\
2. The Public RSA Key is invalid.\nClient ID: " << client->get_id() << std::endl;
		return false;
	case general_error:
		std::cerr << "ERROR: General server failure" << std::endl;
		return false;
	}
}

ResponseCodes ResponseParser::get_code(const std::array<char, 2>& code)
{
	return static_cast<ResponseCodes>(bytes_to_short(code));
}

unsigned short ResponseParser::bytes_to_short(const std::array<char, 2>& bytes)
{
	return static_cast<unsigned short>(bytes[1] << 8) + static_cast<unsigned short>(bytes[0]);
}

unsigned int ResponseParser::bytes_to_int(const std::array<char, 4>& bytes)
{
	return static_cast<unsigned short>(bytes[3] << 24) +
		static_cast<unsigned short>(bytes[2] << 16) +
		static_cast<unsigned short>(bytes[1] << 8) + 
		static_cast<unsigned short>(bytes[0]);
}
