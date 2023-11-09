#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>

#define UNKNOWN_ERROR 0xFFFFFFFF

enum ResponseSizes
{
	size_version = 1,
	size_code = 2,
	size_payload_size = 4,
	size_min_payload = 16
};

enum ResponseCodes {
	register_success = 2100,
	register_fail = 2101,
	public_key_success = 2102,
	valid_crc = 2103,
	message_success = 2104,
	login_success = 2105,
	login_fail = 2106,
	general_error = 2107
};

class ResponseParser
{
public:
	ResponseParser() = default;
	std::vector<char> parse_response(std::vector<char> response);
	static unsigned short bytes_to_short(const std::vector<char>& bytes);
};
