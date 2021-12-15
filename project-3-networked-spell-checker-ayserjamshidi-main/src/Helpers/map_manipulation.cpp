//
// Created by ayserjamshidi on 11/3/21.
//
#include <unordered_map>

#include "helpers.h"

void Helpers::MapManipulation::CreateHashMap(const std::string& str, std::unordered_map<std::string, std::string> &map,  char delim) {
	// Find the delimiter
	size_t pos = str.find(delim);

	// If the delimiter exists, we put everything to the left as its key and everything to the right as its value
	if (pos != std::string::npos)
		map[str.substr(0, pos)] = str.substr(pos + 1, str.size() - 1);
}
