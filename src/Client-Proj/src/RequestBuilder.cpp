#include "RequestBuilder.h"

#include <iostream>

RequestBuilder::RequestBuilder(const char* unique_id, const char version)
{
	std::fill(std::begin(const_headers), std::end(const_headers), 0);
	std::copy_n(unique_id, size_req_client_id, std::begin(const_headers));
	const_headers[size_req_client_id] = version;
	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(const_headers_backup));
	// TODO remember to backup headers after every req built
}


void RequestBuilder::set_client_id(char* unique_id)
{
	std::copy_n(unique_id, size_req_client_id, std::begin(const_headers));
	std::copy_n(unique_id, size_req_client_id, std::begin(const_headers_backup));
}

std::vector<char> RequestBuilder::build_req_register(char* name)
{
	add_fields_to_header(code_register, size_name);
	std::vector<char> req(size_headers + size_name);
	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(req));
	std::copy_n(name, size_name, std::begin(req) + size_headers);
	return req;
}

std::vector<char> RequestBuilder::build_req_send_public_key(char* name, char* public_key)
{
	add_fields_to_header(code_public_key, size_name + size_public_key);
	std::vector<char> req(size_headers + size_name + size_public_key);
	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(req));
	std::copy_n(name, size_name, std::begin(req) + size_headers);
	std::copy_n(public_key, size_public_key, std::begin(req) + size_headers + size_name);
	return req;
}

std::vector<char> RequestBuilder::build_req_login(char* name)
{
	add_fields_to_header(code_login, size_name);
	std::vector<char> req(size_headers + size_name);
	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(req));
	std::copy_n(name, size_name, std::begin(req) + size_headers);
	return req;
}

std::vector<char> RequestBuilder::build_req_send_file(unsigned content_size, char* file_name, std::vector<char> content)
{
	constexpr int content_size_bytes = 4;
	add_fields_to_header(code_valid_crc, size_name + size_req_payload_size);
	std::vector<char> req(size_headers + content_size_bytes + size_name + content_size);
	std::array<char, 4> content_size_converted = int_to_bytearray(content_size);

	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(req));
	std::copy(std::begin(content_size_converted), std::end(content_size_converted), std::begin(req) + size_headers);
	std::copy_n(file_name, size_name, std::begin(req) + size_headers + size_req_payload_size);
	std::copy(content.begin(), content.end(), 
		std::begin(req) + size_headers + size_req_payload_size + size_name);
	return req;
}

std::vector<char> RequestBuilder::build_req_valid_crc(char* file_name)
{
	add_fields_to_header(code_valid_crc, size_name);
	std::vector<char> req(size_headers + size_name);
	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(req));
	std::copy_n(file_name, size_name, std::begin(req) + size_headers);
	return req;
}

std::vector<char> RequestBuilder::build_req_invalid_crc(char* file_name)
{
	add_fields_to_header(code_invalid_crc, size_name);
	std::vector<char> req(size_headers + size_name);
	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(req));
	std::copy_n(file_name, size_name, std::begin(req) + size_headers);
	return req;
}

std::vector<char> RequestBuilder::build_req_final_invalid_crc(char* file_name)
{
	add_fields_to_header(code_final_invalid_crc, size_name);
	std::vector<char> req(size_headers + size_name);
	std::copy(std::begin(const_headers), std::end(const_headers), std::begin(req));
	std::copy_n(file_name, size_name, std::begin(req) + size_headers);
	return req;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void RequestBuilder::add_fields_to_header(const Codes code, const unsigned int payload_size)
{
	std::array<char, size_req_code> converted_code;
	converted_code[1] = static_cast<char>((code & 0xFF00) >> 8);
	converted_code[0] = static_cast<char>(code & 0x00FF);
	std::array<char, size_req_payload_size> converted_payload_size = int_to_bytearray(payload_size);
	std::copy(std::begin(converted_code), std::end(converted_code), std::begin(const_headers) + 17);
	std::copy(std::begin(converted_payload_size), std::end(converted_payload_size), std::begin(const_headers) + 19);
}

std::array<char, 4> RequestBuilder::int_to_bytearray(int to_convert)
{
	std::array<char, 4> converted_payload_size;
	converted_payload_size[3] = static_cast<char>((to_convert & 0xFF000000) >> 24);
	converted_payload_size[2] = static_cast<char>((to_convert & 0x00FF0000) >> 16);
	converted_payload_size[1] = static_cast<char>((to_convert & 0x0000FF00) >> 8);
	converted_payload_size[0] = static_cast<char>(to_convert & 0x000000FF);

	return converted_payload_size;
}
