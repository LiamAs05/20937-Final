#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class Utils
{
public:
	static unsigned long memcrc(char* content, size_t size);
	static std::string read_file(const std::string& path);
	static void write_file(const std::string& path, const std::string& content);
	static std::vector<std::string> split_lines(const std::string& content);
};

