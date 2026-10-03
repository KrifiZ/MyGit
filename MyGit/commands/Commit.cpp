#include "Commit.h"
#include "../core/Objects.h"
#include <ctime>
#include <fstream>
#include <iostream>

std::string writeTree(const Index& entries) {
	std::map<std::string, std::string> lines; 
	std::map<std::string, Index> subdirs;     

	for (const auto& [path, hash] : entries) {
		size_t slash = path.find('/');
		if (slash == std::string::npos) {
			lines[path] = "100644 " + path + '\0' + hexToRaw(hash);
		}
		else {
			std::string dir = path.substr(0, slash);
			std::string subPath = path.substr(slash + 1);
			subdirs[dir][subPath] = hash;
		}
	}

	for (const auto& [dir, subEntries] : subdirs) {
		std::string subHash = writeTree(subEntries);
		lines[dir + "/"] = "40000 " + dir + '\0' + hexToRaw(subHash);

	}

	std::string content;
	for (const auto& [key, line] : lines) {
		content += line;
	}
	return writeObject(type::TREE, content);
}

const std::string AUTHOR = "Imie Nazwisko <ty@example.com>";

std::string headRefPath() {
	std::string head = readFile(".mygit/HEAD");
	std::string ref = ".mygit/" + head.substr(5);

	while (ref.ends_with('\n') || ref.ends_with('\r')) {
		ref.pop_back();
	}

	return ref;
}

std::string readHeadCommit() {
	std::string content = readFile(headRefPath());
	
	if (content.empty()) {
		return "";
	}

	return content.substr(0, 40);
}

std::string commit(const std::string& message) {
	Index index = loadIndex();
	if (index.empty()) {
		std::cerr << "Nothing to commit\n";
		exit(1);
	}

	std::string tree = writeTree(index);
	std::string parent = readHeadCommit();
	std::string signature = AUTHOR + " " + std::to_string(std::time(nullptr)) + " +0000";

	std::string content;

	content += "tree " + tree + '\n';
	if (!parent.empty()) {
		content += "parent " + parent + '\n';
	}
	content += "author " + signature + '\n';
	content += "committer " + signature + '\n';
	content += '\n';
	content += message + '\n';

	std::string hash = writeObject(type::COMMIT, content);
	std::ofstream file(headRefPath(), std::ios_base::binary);
	file << hash << '\n';
	
	return hash;
}