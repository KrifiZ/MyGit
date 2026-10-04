#pragma once
#include <string>

struct Commit {
	std::string tree;
	std::string parent;
	std::string author;
	std::string message;
};

Commit parseCommit(const std::string& content);
void log();
