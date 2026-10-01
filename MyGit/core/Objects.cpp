#include "Objects.h"
#include "../utils/ZLibWrapper.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

std::string readFile(const std::string& path) {
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		std::cerr << "Cannot open " << path << '\n';
		exit(1);
	}

	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

std::string buildObject(type t, const std::string& content) {
	std::string result;
	//blob 3\0hi\n
	result = std::string(to_string(t)) + " " + std::to_string(content.length()) + '\0' + content;
	return result;
}

std::string hashObject(type t, const std::string& content) {
	return Crypto().sha1Hex(buildObject(t, content));
}

std::string writeObject(type t, const std::string& content) {
	std::string hash = hashObject(t, content);
	std::string dir = std::string(".mygit/objects/") + hash.substr(0, 2);
	std::string path = dir + "/" + hash.substr(2);

	
	if (std::filesystem::exists(path)) {
		return hash;
	}

	std::filesystem::create_directories(dir);
	std::ofstream file(path, std::ios::binary);
	file << ZLibWrapper().compressData(buildObject(t, content));
	return hash;
}

Object readObject(const std::string& hash) {
	return Object();
}

std::string rawToHex(const std::string& raw) {
	std::ostringstream oss;
	for (unsigned char byte : raw) {
		oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
	}
	return oss.str();
}

std::string hexToRaw(const std::string& hex) {
	return "";
}