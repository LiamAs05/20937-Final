#include "Client.h"
#include "ResponseParser.h"

int main()
{
	auto s = ResponseParser();
	std::vector<char> v = { 0x00, 0x01, 0x02, 0x03 };
	s.parse_response(v);
}