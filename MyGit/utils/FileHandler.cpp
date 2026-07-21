#include "FileHandler.h"

void FileHandler::OnSpecificFiles(const std::vector<std::string>& paths, void(*callback)(const std::string& path)){
	for (const std::string& path : paths){
		callback(path);
	}
}

void FileHandler::OnSpecificFile(const std::string& path, void(*callback)(const std::string& path)) {
	callback(path);
}

