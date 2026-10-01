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
	return "";
}
