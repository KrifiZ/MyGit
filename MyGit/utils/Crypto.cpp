#include "Crypto.h"
#include <openssl/evp.h>
#include <fstream>
#include <sstream>
#include <iomanip>

const char* to_string(type t) {
	switch (t) {
		case type::BLOB: return "blob";
		case type::COMMIT: return "commit";
		case type::TREE: return "tree";
		default: return "unknown";
	}
}

std::string Crypto::generateSum(type t, const std::string& path) {
	EVP_MD_CTX* mdctx;
	const EVP_MD* md;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	md = EVP_get_digestbyname("sha1");
	if (md == NULL) {
		printf("Unknown message digest\n");
		exit(1);
	}

	mdctx = EVP_MD_CTX_new();
	if (!EVP_DigestInit_ex2(mdctx, md, NULL)) {
		printf("Message digest initialization failed.\n");
		EVP_MD_CTX_free(mdctx);
		exit(1);
	}

	std::ifstream ifFile(path);
	if (!ifFile) {
		printf("Error opening main file for writing\n");
		exit(1);
	}

	std::string str;
	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];

	ifFile.seekg(0, std::ios::end);
	size_t length = ifFile.tellg();
	ifFile.seekg(0, std::ios::beg);

	switch (t)
	{
	case type::BLOB: {
		std::string str = std::string("blob") + ' ' + std::to_string(length) + '\0';
		printf("%s", str.c_str());
		EVP_DigestUpdate(mdctx, str.c_str(), str.size());
		break;
	}
	case type::COMMIT:
		break;
	case type::TREE:
		break;
	default:
		break;
	}


	while (ifFile.read(buffer, BUFFER_SIZE)) {
		if (!EVP_DigestUpdate(mdctx, buffer, ifFile.gcount())) {
			printf("Message digest update failed.\n");
			EVP_MD_CTX_free(mdctx);
			exit(1);
		}
	}

	if (ifFile.gcount() > 0) {
		if (!EVP_DigestUpdate(mdctx, buffer, ifFile.gcount())) {
			printf("Message digest update failed.\n");
			EVP_MD_CTX_free(mdctx);
			exit(1);
		}
	}

	if (!EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
		printf("Message digest finalization failed.\n");
		EVP_MD_CTX_free(mdctx);
		exit(1);
	}
	EVP_MD_CTX_free(mdctx);

	printf("Digest is: ");

	for (unsigned int i = 0; i < md_len; i++)
		printf("%02x", md_value[i]);

	std::ostringstream oss;
	for (unsigned int i = 0; i < md_len; i++) {
		oss << std::setw(2) << std::setfill('0') << std::hex
			<< static_cast<int>(md_value[i]);
	}
	std::string result = oss.str();
	return result;
}