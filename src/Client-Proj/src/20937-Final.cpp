#include "Utils.h"

int main()
{
	std::cout << "Client starting..." << std::endl;
	//auto c = Client();
	const char* content = "abcd";
	std::cout << Utils::memcrc((char*)content, 4) << std::endl;
}