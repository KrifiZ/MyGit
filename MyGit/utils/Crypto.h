#pragma once
#include <string>

enum type {
	BLOB,
	COMMIT,
	TREE
};

class Crypto {
public:
	std::string generateSum(type t, const std::string& path);
};