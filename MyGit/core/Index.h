#pragma once
#include <map>
#include <string>
#include <vector>

// ponytail: tekstowy .mygit/index.txt "<hash> <ścieżka>"; prawdziwy git ma binarny .git/index z metadanymi
using Index = std::map<std::string, std::string>;

Index loadIndex();
void saveIndex(const Index& index);

std::string normalizePath(const std::string& path);
std::vector<std::string> listWorkingFiles();
bool isIgnored(const std::string& path);