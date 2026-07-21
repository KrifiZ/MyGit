#include "Add.h"
#include <memory>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <zlib.h>
#include "../utils/ZLibWrapper.h"

void Add::addBlob(const std::string& path) {
	std::unique_ptr<Crypto> cryptoPtr = std::make_unique<Crypto>();
	std::string sum = cryptoPtr->generateSum(type::BLOB, path);

	if (!std::filesystem::exists(".mygit/objects")) {
		std::cerr << "Repository is not initialized\n";
		exit(1);
	}

	std::string hashPath = std::string(".mygit/objects/") + sum.at(0) + sum.at(1);
	
	if (!std::filesystem::exists(hashPath)){
		std::error_code ec;
		std::filesystem::create_directory(hashPath, ec);
		if (ec) {
			std::cerr << "Cannot create a hash blob folder: " << ec.message() << "\n";
			exit(1);
		}
	}

	std::unique_ptr<ZLibWrapper> zLib = std::make_unique<ZLibWrapper>();
	zLib->compressBlob(sum, path);
}