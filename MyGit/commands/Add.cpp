#include "Add.h"
#include <memory>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <zlib.h>

void Add::addBlob(const std::string& path) {
	std::unique_ptr<Crypto> cryptoPtr = std::make_unique<Crypto>();
	std::string sum = cryptoPtr->generateSum(type::BLOB, path);
	if (std::filesystem::exists(".mygit/objects")) {
		std::cerr << "Repository is not initialized\n";
		exit(1);
	}
	std::string hashPath = ".mygit/objects" + sum.at(0) + sum.at(1);
	if (!std::filesystem::exists(hashPath)){

		int check = std::filesystem::create_directory(hashPath);
		if (!check) {
			std::cerr << "Cannot create a hash blob folder\n";
			exit(1);
		}
	}
}