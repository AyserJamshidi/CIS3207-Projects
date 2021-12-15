//
// Created by ayserjamshidi on 11/3/21.
//
#include <iostream>

#include "helpers.h"

void Helpers::Print::HashTable(std::unordered_map<std::string, std::string> const &m) {
	for (auto const &pair: m)
		std::cout << "{" << pair.first << ": " << pair.second << "}" << std::endl;
}