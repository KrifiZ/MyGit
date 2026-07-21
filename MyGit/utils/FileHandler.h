#pragma once
#include <string>
#include <vector>

class FileHandler {
	void OnSpecificFiles(const std::vector<std::string>& paths, void(*callback)(const std::string& path));
	void OnSpecificFile(const std::string& path, void(*callback)(const std::string& path));
	void OnAllFiles(void (*callback));
};