#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include <fstream>
#include "MyGit.h"
#include "../commands/Add.h"


int main(int argc, char* argv[])
{
	std::cin.get();
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

	if (command == "add") {
		std::unique_ptr<Add> add = std::make_unique<Add>();
		if (strcmp(argv[2], ".") == 0) {
			printf("Work in progress\n");
			exit(1);
		}

		for (size_t i = 2; i < argc; i++){
			const std::string& path = argv[i];
			add->addBlob(path);
		}
	}

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