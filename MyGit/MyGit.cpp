#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include "MyGit.h"
#include <fstream>
#include <openssl/evp.h>

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Invalid arguments\n";
		return 1;
	}

	std::string_view command = argv[1];

	if (command == "init") {

		if (InitializeRepository()) {
			return 1;
		}

		std::cout << "Hello CMake.\n";
	}
	
	hash(R"(C:\Users\micha\Desktop\Robert M. Wegner - Opowiesci z meekhanskiego pogranicza Polnoc-Poludnie czyta F.Kosior 96kbps\01.mp3)");

	return 0;
}

bool InitializeRepository() {
	if (std::filesystem::exists(".mygit")) {
		std::cerr << "Repository is already initialized\n";
		return 1;
	}

	int check = std::filesystem::create_directory(".mygit");
	if (!check) {
		std::cerr << "Cannot create a root folder\n";
		return 1;
	}

	check = std::filesystem::create_directory("./.mygit/objects");
	if (!check) {
		std::cerr << "Cannot create a objects folder\n";
		return 1;
	}

	check = std::filesystem::create_directory("./.mygit/refs");
	if (!check) {
		std::cerr << "Cannot create a refs folder\n";
		return 1;
	}

	check = std::filesystem::create_directory("./.mygit/refs/heads");
	if (!check) {
		std::cerr << "Cannot create a refs/heads folder\n";
		return 1;
	}

	std::ofstream outFile("./.mygit/refs/heads/main");
	if (!outFile) {
		std::cerr << "Error opening main file for writing\n";
		return 1;
	}
	outFile.close();

	outFile.open("./.mygit/HEAD");
	if (!outFile) {
		std::cerr << "Error opening HEAD file for writing\n";
		return 1;
	}
	
	outFile << "ref: refs/heads/main";
	outFile.close();

	return 0;
}

void hash(std::string path) {
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
		std::cerr << "Error opening main file for writing\n";
		exit(1);
	}

	std::string str;
	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];

	while(ifFile.read(buffer, BUFFER_SIZE)) {
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
	printf("\n");
}