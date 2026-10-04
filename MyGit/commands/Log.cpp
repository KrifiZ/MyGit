#include "Log.h"
#include "Commit.h"
#include "../core/Objects.h"
#include <iostream>
#include <sstream>

Commit parseCommit(const std::string& content) {
	Commit commit;
	std::istringstream stream(content);
	std::string line;

	const std::pair<std::string_view, std::string&> fields[] = {
		{"tree ", commit.tree },
		{"parent ", commit.parent},
		{"author ", commit.author}
	};

	while (std::getline(stream, line)) {
		if (line.empty()) {
			std::getline(stream, commit.message, '\0');
			break;
		}
	
		for (const auto& [prefix, target] : fields){
			if (line.starts_with(prefix)) {
				target = line.substr(prefix.size());
			}
		}
	}
	return commit;
}

void log() {
	std::string hash = readHeadCommit();
	while (!hash.empty()) {
		Commit commit = parseCommit(readObject(hash).content);
		std::cout << "commit " << hash << "\n";
		std::cout << "Author: " << commit.author << "\n\n";
		std::cout << "    " << commit.message << "\n";

		hash = commit.parent;
	}
}