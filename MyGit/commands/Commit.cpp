#include "Commit.h"
#include "../core/Objects.h"
#include <ctime>
#include <fstream>
#include <iostream>

std::string writeTree(const Index& entries) {
	std::map<std::string, std::string> lines; 
	std::map<std::string, Index> subdirs;     

	for (const auto& [path, hash] : entries) {
		size_t slash = path.find('/');
		if (slash == std::string::npos) {
			lines[path] = "100644 " + path + '\0' + hexToRaw(hash);
		}
		else {
			std::string dir = path.substr(0, slash);
			std::string subPath = path.substr(slash + 1);
			subdirs[dir][subPath] = hash;
		}
	}

	for (const auto& [dir, subEntries] : subdirs) {
		std::string subHash = writeTree(subEntries);
		lines[dir + "/"] = "40000 " + dir + '\0' + hexToRaw(subHash);

	}

	std::string content;
	for (const auto& [key, line] : lines) {
		content += line;
	}
	return writeObject(type::TREE, content);
}