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
	std::array<char, size_req_client_id> uid;
	std::vector<unsigned char> aes;
	std::array<char, 4> res_crc;
	if (payload_size) {
		client->recv(payload.data(), payload_size);
		aes.resize(payload_size - 16);
	}

	switch (code)
	{
	case register_success:
		std::copy_n(payload.begin(), size_req_client_id, uid.begin());
		client->set_id(uid);
		return true;
	case register_fail:
		std::cerr << "Registration has failed, restart as a new client..." << std::endl;
		return false;
	case public_key_success: case login_success:
		std::copy(payload.begin() + 16, payload.end(), aes.begin());
		client->set_aes_key(aes);
		std::cout << "AES Key of length " << aes.size() << " starting with bytes "
			<< std::hex << +aes[0] << +aes[1] << +aes[2] << +aes[3] << " received" << std::dec << std::endl;
		return true;
	case valid_crc:
		std::copy_n(payload.end() - 4, 4, res_crc.begin());
		if (client->crc ==  bytes_to_int(res_crc))
		{
			return true;
		}
		return false;
	case message_success:
		std::copy_n(payload.begin(), size_req_client_id, uid.begin());
		client->set_id(uid);
		std::cout << "This concludes our session, Goodbye client " << client->get_id();
		return false;
	case login_fail:
		std::copy_n(payload.begin(), size_req_client_id, uid.begin());
		client->set_id(uid);
		std::cerr << "Login has failed, this can happen either when\n\
1. The username already exists in the DB\n\
2. The Public RSA Key is invalid.\nClient ID: " << client->get_id() << std::endl;
		return false;
	case general_error:
		std::cerr << "ERROR: General server failure OR Invalid File CRC" << std::endl;
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
	return (static_cast<unsigned char>(bytes[3]) << 24) +
		(static_cast<unsigned char>(bytes[2]) << 16) +
		(static_cast<unsigned char>(bytes[1]) << 8) + 
		static_cast<unsigned char>(bytes[0]);
}