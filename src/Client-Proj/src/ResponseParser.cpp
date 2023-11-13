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

	switch (code)
	{
	case register_success:
		std::array<char, size_req_client_id> uid;
		client->recv(uid.data(), size_req_client_id);
		client->set_id(uid);
		return true;
	case register_fail:
		std::cout << "Registration has failed, restart as a new client..." << std::endl;
		return false;
	case public_key_success:
		break;
	case valid_crc:
		break;
	case message_success:
		break;
	case login_success:
		break;
	case login_fail:
		break;
	case general_error:
		return false;
	default:
		std::cout << "This is totally broken and shouldn't even be possible";
		return false;
	}
	return false;
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
