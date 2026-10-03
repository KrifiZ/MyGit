#pragma once
#include <string>
#include "../core/Index.h"

std::string writeTree(const Index& entries);
std::string headRefPath();
std::string readHeadCommit();
std::string commit(const std::string& message);