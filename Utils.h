#pragma once
#include <string>

// parse integer from string, return true if successful
bool parseInt(const std::string& s, int& out);

// trim whitespace at both ends
std::string trim(const std::string& s);