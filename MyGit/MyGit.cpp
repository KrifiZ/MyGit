#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include "MyGit.h"
#include <fstream>

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Invalid arguments\n";
		return 1;
	}

	std::string_view command = argv[1];

	if (command == "init") {

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

		std::cout << "Hello CMake.\n";
	}
	
	return 0;
}
