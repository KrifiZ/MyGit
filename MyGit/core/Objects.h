#pragma once
#include <string>
#include "../utils/Crypto.h"

struct Object {
	std::string kind; // blob, tree, commit
	std::string content;
};

std::string readFile(const std::string& path);
std::string buildObject(type t, const std::string& content);
std::string hashObject(type t, const std::string& content);
std::string writeObject(type t, const std::string& content);
Object readObject(const std::string& hash);

std::string rawToHex(const std::string& raw);
std::string hexToRaw(const std::string& hex);
