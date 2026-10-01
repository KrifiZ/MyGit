#include "ZLibWrapper.h"
#include <fstream>
#include <zlib.h>
#include <iostream>
#include <vector>

std::string ZLibWrapper::compressData(const std::string& data) {
	uLongf destinationLength = compressBound(data.size());
	std::string dest(destinationLength, '\0');
	int ret = compress(reinterpret_cast<Bytef*>(dest.data()), &destinationLength,
		reinterpret_cast<const Bytef*>(data.data()), data.size());

	if (ret != Z_OK) {
		std::cerr << "Compression error (" << ret << "): " << zError(ret) << '\n';
		exit(1);
	}
	
	dest.resize(destinationLength);
	return dest;
}

std::string ZLibWrapper::decompressData(const std::string& data) {
	std::string dest(data.size() * 4 + 64, '\0');
	int ret = Z_OK;

	do {
		uLongf destinationLength = dest.size();
		ret = uncompress(reinterpret_cast<Bytef*>(dest.data()), &destinationLength, reinterpret_cast<const Bytef*>(data.data()), static_cast<uLongf>(data.size()));
		if (ret == Z_OK) {
			dest.resize(destinationLength);
		}
		else if (ret == Z_BUF_ERROR) {
			dest.resize(dest.size() * 2);
		}
	} while (ret == Z_BUF_ERROR);

	if (ret != Z_OK) {
		std::cerr << "decompression error: " << ret << "\n";
		exit(1);
	}
	return dest;
}
