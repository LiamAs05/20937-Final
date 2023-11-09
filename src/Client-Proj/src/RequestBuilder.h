#pragma once
#include <array>
#include <vector>
#include <algorithm>

enum HeaderSizes
{
	size_name = 255,
	size_public_key = 160,
	size_headers = 23,
	size_req_client_id = 16,
	size_req_code = 2,
	size_req_payload_size = 4
};

enum RequestCodes
{
	code_register = 1025,
	code_public_key = 1026,
	code_login = 1027,
	code_send_file = 1028,
	code_valid_crc = 1029,
	code_invalid_crc = 1030,
	code_final_invalid_crc = 1031,
};

class RequestBuilder
{
public:
	RequestBuilder() = delete;
	RequestBuilder(const char* unique_id, char version);
	void set_client_id(char* unique_id);
	std::vector<char> build_req_register(char* name);
	std::vector<char> build_req_send_public_key(char* name, char* public_key);
	std::vector<char> build_req_login(char* name);
	std::vector<char> build_req_send_file(unsigned int content_size,
		char* file_name,
		std::vector<char> content
	);
	std::vector<char> build_req_valid_crc(char* file_name);
	std::vector<char> build_req_invalid_crc(char* file_name);
	std::vector<char> build_req_final_invalid_crc(char* file_name);


private:
	void add_fields_to_header(RequestCodes code, unsigned int payload_size);
	static std::array<char, 4> int_to_bytearray(int to_convert);
	std::array<char, size_headers> const_headers;
	std::array<char, size_headers> const_headers_backup;
};

