// MyGit.cpp : Defines the entry point for the application.
//
#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include "MyGit.h"

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
			std::cerr << "Cannot create a folder\n";
			return 1;
		}
		std::cout << "Hello CMake.\n";
	}
	
	return 0;
}
