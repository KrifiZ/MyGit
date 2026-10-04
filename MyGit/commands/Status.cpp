#include "Status.h"
#include "Commit.h"
#include "Log.h"
#include "../core/Objects.h"
#include <iostream>

void readTree(const std::string& treeHash, const std::string& prefix, Index& out) {
	std::string content = readObject(treeHash).content;
	size_t pos = 0;
	while (pos < content.size()) {
		std::string mode;
		std::string name;
		std::string hash;
		size_t next = content.size();
		size_t space = content.find(' ', pos);
		size_t nul = content.find('\0', space);

		mode = content.substr(pos, space - pos);
		name = content.substr(space + 1, nul - space - 1);
		hash = rawToHex(content.substr(nul + 1, 20));
		next = nul + 1 + 20;

		if (mode == "40000") {
			readTree(hash, prefix + name + "/", out);
		}
		else {
			out[prefix + name] = hash;
		}

		pos = next;
	}
}

void status() {
	Index index = loadIndex();

	Index head;
	std::string headCommit = readHeadCommit();
	if (!headCommit.empty()) {
		readTree(parseCommit(readObject(headCommit).content).tree, "", head);
	}

	Index working;
	for (const std::string& path : listWorkingFiles()) {
		working[path] = hashObject(type::BLOB, readFile(path));
	}

	std::cout << "Changes to be committed:\n";
	for (const auto& [path, hash] : index) {
		if (!head.contains(path)) {
			std::cout << "  new file: " << path << '\n';
		}
		else if(head.at(path) != hash) {
			std::cout << "  modified: " << path << '\n';
		}

	}
	for (const auto& [path, hash] : head) {
		if (!index.contains(path)) {
			std::cout << "  deleted: " << path << '\n';
		}
	}

	std::cout << "\nChanges not staged for commit:\n";
	for (const auto& [path, hash] : index) {
		if (!working.contains(path)) {
			std::cout << "  deleted: " << path << '\n';
		}
		else if (working.at(path) != hash) {
			std::cout << "  modified: " << path << '\n';
		}
	}

	std::cout << "\nUntracked files:\n";
	for (const auto& [path, hash] : working) {
		if (!index.contains(path)) {
			std::cout << "  " << path << '\n';
		}
	}
}
