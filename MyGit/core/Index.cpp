#include "Index.h"
#include <filesystem>
#include <fstream>

Index loadIndex() {
	Index index;
	std::ifstream file(".mygit/index.txt");
	std::string line;
	while (std::getline(file, line)) {
		std::string hash, path;
		hash = line.substr(0, line.find(' '));
		path = line.substr(line.find(' ') + 1);
		index[path] = hash;
	}
	return index;
}

void saveIndex(const Index& index) {
	std::ofstream file(".mygit/index.txt");
	for (const auto& [path, hash] : index) {
		file << hash << ' ' << path << '\n';
	}
}

std::string normalizePath(const std::string& path) {
	return std::filesystem::relative(path).generic_string();
}

std::vector<std::string> listWorkingFiles() {
	std::vector<std::string> files;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(".")) {
		std::string path = normalizePath(entry.path().string());
		if (!entry.is_regular_file() || path.starts_with(".mygit/")  || path.starts_with(".git/") ){
			continue;
		}

		files.push_back(path);
	}
	return files;
}

bool isIgnored(const std::string& path) {
	std::ifstream file(".mygitignore");
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line.starts_with('#')) {
			continue;
		}
		if (line == path) {
			return true;
		}
		if (line.ends_with('/') && path.starts_with(line) ){
			return true;
		}
		if (line.starts_with('*')) {
			if (path.ends_with(line.substr(1))) {
				return true;
			}
		}
	}
	return false;
}