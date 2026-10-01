#pragma once
#include <string>

enum type {
	BLOB,
	COMMIT,
	TREE
};

const char* to_string(type t);

class Crypto {
public:
	std::string sha1Hex(const std::string& data);
};