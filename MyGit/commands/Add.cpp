#include "Add.h"
#include "../core/Objects.h"
#include <filesystem>
#include <iostream>

void Add::addBlob(const std::string& path) {
	if (!std::filesystem::exists(".mygit/objects")) {
		std::cerr << "Repository is not initialized\n";
		exit(1);
	}

	std::string hash = writeObject(type::BLOB, readFile(path));
	std::cout << hash << " " << path << "\n";
}