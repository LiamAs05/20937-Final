#include "RequestBuilder.h"

RequestBuilder::RequestBuilder(std::array<std::byte, size_req_client_id> unique_id, std::byte version):
	unique_id_(unique_id), version_(version)
{
	std::copy_n(unique_id.data(), unique_id.data() + unique_id.size(), const_headers.data());
	const_headers[unique_id.size()] = version;
}

std::vector<std::byte> RequestBuilder::build_req_register(std::array<std::byte, MAX_NAME_SIZE> name)
{
	std::vector<std::byte> code;
}
