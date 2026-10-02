#include "Add.h"
#include "../core/Objects.h"
#include <iostream>

void Add::addBlob(const std::string& path, Index& index) {
	std::string hash = writeObject(type::BLOB, readFile(path));
	index[normalizePath(path)] = hash;
	std::cout << hash << " " << normalizePath(path) << "\n";
}