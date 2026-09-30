#pragma once
#include <string>

class ZLibWrapper {
public:
	std::string compressData(const std::string& data);
	std::string decompressData(const std::string& data);
};