#include "ZLibWrapper.h"
#include <fstream>
#include <zlib.h>
#include <iostream>
#include <vector>

void ZLibWrapper::compressBlob(const std::string& hash,const std::string& path) {
	std::ifstream ifFile(path, std::ios::binary);
	if (!ifFile) {
		printf("Error opening main file for reading\n");
		exit(1);
	}

	ifFile.seekg(0, std::ios::end);
	size_t length = ifFile.tellg();
	ifFile.seekg(0, std::ios::beg);

	std::string blobHeader = std::string("blob") + ' ' + std::to_string(length) + '\0';
	std::string source = blobHeader;
	source.resize(source.size() + length);
	ifFile.read(source.data() + blobHeader.size(), length);
	uLong sourceLength = source.size();
	uLong destinationLength = compressBound(sourceLength);
	std::vector<Bytef> dest(destinationLength);

	int ret = compress(dest.data(), &destinationLength, reinterpret_cast<Bytef*>(source.data()), sourceLength);

	if (ret != Z_OK) {
		std::cerr << "compression error: " << ret << "\n";
		exit(1);
	}

	dest.resize(destinationLength);
	std::ofstream ofFile(std::string(".mygit/objects/") + hash.substr(0, 2) + "/" + hash.substr(2), std::ios::binary);
	if (!ofFile) {
		printf("Error opening main file for writing\n");
		exit(1);
	}
	ofFile.write(reinterpret_cast<const char*>(dest.data()), dest.size());
}