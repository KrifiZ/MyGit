#include "Crypto.h"
#include <openssl/evp.h>
#include <openssl/err.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "../core/Objects.h"

const char* to_string(type t) {
	switch (t) {
		case type::BLOB: return "blob";
		case type::COMMIT: return "commit";
		case type::TREE: return "tree";
		default: return "unknown";
	}
}

std::string Crypto::sha1Hex(const std::string& data) {
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len = 0;
	auto ok = EVP_Digest(data.data(), data.size(), md_value, &md_len, EVP_sha1(), nullptr);
	
	if (!ok) {
		std::cerr << "EVP_Digest failed: "
			<< ERR_error_string(ERR_get_error(), nullptr) << '\n';
		exit(1);
	}
	return rawToHex(std::string(reinterpret_cast<char*>(md_value), md_len));
}
