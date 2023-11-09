#include "ResponseParser.h"

std::vector<char> ResponseParser::parse_response(std::vector<char> response)
{
	/*if (response.size() < size_code + size_payload_size + size_version + size_min_payload)
	{
		std::cerr << "An unknown error occurred, invalid response from server." << std::endl;
		_exit(UNKNOWN_ERROR);
	}*/
	std::vector<char> code(2);
	std::copy_n(response.begin() + 1, 2, code.begin());
	std::cout << bytes_to_short(code);
}

unsigned short ResponseParser::bytes_to_short(const std::vector<char>& bytes)
{
	return static_cast<unsigned short>(bytes[1] << 8) + static_cast<unsigned short>(bytes[0]);
}
