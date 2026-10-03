#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include <fstream>
#include "MyGit.h"
#include "../commands/Add.h"
#include "Objects.h"
#include <io.h>
#include <fcntl.h>
#include "../commands/Commit.h"

int main(int argc, char* argv[])
{
	_setmode(_fileno(stdout), _O_BINARY);

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
		if (argc < 3) {
			std::cerr << "Invalid arguments\n";
			return 1;
		}
		if (!std::filesystem::exists(".mygit/objects")) {
			std::cerr << "Repository is not initialized\n";
			return 1;
		}

		Index index = loadIndex();
		Add add;
		if (std::string_view(argv[2]) == ".") {
			for (const std::string& path : listWorkingFiles()) {
				add.addBlob(path, index);
			}
		}
		else {
			for (int i = 2; i < argc; i++) {
				add.addBlob(argv[i], index);
			}
		}
		saveIndex(index);
	}

	if (command == "hash-object") {
		if (argc < 3) {
			std::cerr << "Invalid arguments\n";
			return 1;
		}
		std::cout << hashObject(type::BLOB,readFile(argv[2])) << "\n";
	}

	if (command == "cat-file") {
		if (argc < 3 || strlen(argv[2]) != 40) {
			std::cerr << "Usage: mygit cat-file <40-char hash>\n";
			return 1;
		}
		Object obj = readObject(argv[2]);
		std::cout << obj.kind << "\n" << obj.content;
	}

	if (command == "ls-files") {
		for (const auto& [path, hash] : loadIndex()) {
			std::cout << hash << " " << path << "\n";
		}
	}

	if (command == "write-tree") {
		std::cout << writeTree(loadIndex()) << "\n";
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