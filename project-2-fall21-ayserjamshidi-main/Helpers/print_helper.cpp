//
// Created by ayserjamshidi on 10/23/21.
//
#include <iostream>

#include "print_helper.h"
#include "directory_helper.h"

template<typename K, typename V>
void PrintHelper::PrintMap(std::map<K, V> const &m) {
for (auto const &pair: m)
std::cout << "{" << pair.first << ": " << pair.second << "}\n";
}

void PrintHelper::PrintVector(std::vector<std::string> &vect) {
	for (auto const &str: vect)
		std::cout << "[" << str << "] ";

	std::cout << std::endl;
}

void PrintHelper::PrintShellText(const shell& given_shell) {
	std::cout << DirectoryHelper::GetWorkingPath() << given_shell.name << given_shell.name_suffix;
}