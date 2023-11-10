#include "Utils.h"

std::string Utils::read_file(const std::string& path)
{
	std::stringstream ret("");
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

void Utils::write_file(const std::string& path, const std::string& content)
{
	std::ofstream file;

	try 
	{
		file.open(path);
		file << content;
	}
	catch (const std::ifstream::failure& e) {
		std::cerr << "Exception opening/reading "
			<< path << "\n" << e.what() << std::endl;
	}

	file.close();
}

std::vector<std::string> Utils::split_lines(const std::string& content)
{
	std::stringstream ss(content);
	std::vector<std::string> lines;

	if (!content.empty())
	{
		std::string tmp;
		while (std::getline(ss, tmp, '\n')) {
			lines.push_back(tmp);
		}
	}

	return lines;
}
