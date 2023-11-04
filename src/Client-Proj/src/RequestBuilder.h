#pragma once
#include <cstddef>
#include <array>
#include <vector>
#include <algorithm>

#define MAX_NAME_SIZE 255
#define MAX_KEY_SIZE 160
#define HEADER_SIZE 23

enum HeaderSizes
{
	size_req_client_id = 16,
	size_req_code = 2,
	size_req_payload_size = 4
};

enum Codes
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
	RequestBuilder(std::array<std::byte, size_req_client_id> unique_id, std::byte version);



private:
	std::vector<std::byte> build_req_register(std::array<std::byte, MAX_NAME_SIZE> name);
	std::vector<std::byte> build_req_send_public_key(std::array<std::byte, MAX_NAME_SIZE> name, std::array<std::byte, MAX_KEY_SIZE> public_key);
	std::vector<std::byte> build_req_login(std::array<std::byte, MAX_NAME_SIZE> name);
	std::vector<std::byte> build_req_send_file(std::array<std::byte, 4> content_size,
		std::array<std::byte, MAX_NAME_SIZE> file_name,
		std::vector<std::byte> content
	);
	std::vector<std::byte> build_req_valid_crc(std::array<std::byte, MAX_NAME_SIZE> file_name);
	std::vector<std::byte> build_req_invalid_crc(std::array<std::byte, MAX_NAME_SIZE> file_name);
	std::vector<std::byte> build_req_final_invalid_crc(std::array<std::byte, MAX_NAME_SIZE> file_name);

	void add_fields_to_header(Codes code, std::array<std::byte, 4> payload_size);
	/**
	 * \brief Protocol fields
	 */
	std::array<std::byte, size_req_client_id> unique_id_;
	std::byte version_;

	std::array<std::byte, HEADER_SIZE> const_headers;
};

