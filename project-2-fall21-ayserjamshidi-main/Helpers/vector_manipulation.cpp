//
// Created by lmfaoown on 10/20/21.
//

#include <sstream>
#include <algorithm>

#include "vector_manipulation.h"

void VectorManipulation::TokenizeStringInteractiveMode(const std::string &str, std::vector<std::string> &vect, char delim) {
	std::string cur_line;
	std::istringstream ss(str);

	// Split ss into cur_line whenever we encounter delim
	while (std::getline(ss, cur_line, delim))
		vect.push_back(cur_line); // Input the current line into the vector

	// Reset the string stream
	ss.str(std::string());
}

void VectorManipulation::TokenizeStringBatchMode(const std::string &str, std::vector<std::string> &vect, char delim) {
	std::string cur_line;
	std::istringstream ss(str);

	while (std::getline(ss, cur_line, delim)) {
		// If the last character of this string is a space, we removed the string delimiter from the command.  Add it back!
		if (cur_line.at(cur_line.size() - 1) == ' ') {
			vect.push_back(cur_line + "&");
		} else
			vect.push_back(cur_line);
	}

	// Reset the string stream
	ss.str(std::string());
}

int VectorManipulation::FindElementInVector(const std::string &element, std::vector<std::string> &vect) {
	// Do a linear search inside vect for element.  Return -1 if nothing was found.
	for (unsigned int i = 0; i < vect.size(); i++)
		if (element == vect[i])
			return i;

	return -1;
}

void VectorManipulation::CleanTrailingSpaces(std::vector<std::string> &given_vector) {
	for (std::string &cur_string: given_vector) {
		while (!cur_string.empty() && cur_string.at(0) == ' ')
			cur_string.erase(0, 1);

		while (!cur_string.empty() && cur_string.at(cur_string.size() - 1) == ' ')
			cur_string.erase(cur_string.size() - 1, 1);
	}
}