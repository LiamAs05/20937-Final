#include "ResponseParser.h"
#include "Client.h"

ResponseParser::ResponseParser(Client* a)
{
	client = a;
}

std::vector<char> ResponseParser::parse_response(const std::vector<char>& response)
{
	if (response.size() < size_code + size_payload_size + size_version + size_min_payload)
	{
		std::cerr << "An unknown error occurred, invalid response from server." << std::endl;
		_exit(UNKNOWN_ERROR);
	}

	ResponseCodes code = get_code({ response[1], response[2] });
	const int payload_size = bytes_to_int({ response[3], response[4], response[5], response[6] });

	parse_code(code);
}

void ResponseParser::parse_code(ResponseCodes code)
{
	switch (code)
	{
	case register_success:
		std::cout << client->get_name();
		break;
	case register_fail:
		std::cout << client->get_name();
		std::cout << "YAY";
		break;
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
		break;
	default:
		std::cout << "This is totally broken and shouldn't even be possible";
		break;
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
