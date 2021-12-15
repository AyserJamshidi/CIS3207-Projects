//
// Created by ayserjamshidi on 10/29/21.
//

#include "Headers/dictionary.h"

std::ifstream file;
std::unordered_map<std::string, int> dict;
const char *exist = "OK";
const char *non_exist = "MISSPELLED";

int dictionary::LoadFile(char *input) {
	file.open(input);

	if (!file.is_open())
		return false;

	std::string current_word;

	// Loop every line in the file and consider each line to be its own word, inputting it into the dictionary hash table
	while (getline(file, current_word))
		dict[current_word] = 1;

	file.close();

	return dict.size();
}

const char *dictionary::SpellCheck(const char *word) {
	return dict.count(std::string(word)) ? exist : non_exist;
}