#include "Utils.h"


std::string Utils::read_file(const std::string& path)
{
	std::stringstream ret;
	std::ifstream file;

	try {
		std::string content;
		file.open(path);

		while (getline(file, content)) {
			ret << content << "\n";
		}
	}
	catch (const std::ifstream::failure& e) {
		std::cerr << "Exception opening/reading "
		<< path << "\n" << e.what() << std::endl;
	}

	file.close();

	return ret.str();
}
