//
// Created by lmfaoown on 10/6/21.
//

#include <vector>
#include <iostream>
#include <dirent.h>
#include <algorithm>

#include "../Helpers/directory_helper.h"
#include "internal_commands.h"
#include "../Helpers/vector_manipulation.h"

void InternalCommand::dir(const std::string &args) {
	std::vector<std::string> tokens;
	VectorManipulation::TokenizeStringInteractiveMode(args, tokens, ' ');

	// Error check
	if (tokens.size() > 1) {
		std::cout << "You can't have more than one parameter for DIR." << std::endl;
		return;
	}

	// Open directory
	std::string desired_dir_string;
	DIR *desired_dir;

	// If argc == 2 and the argument isn't empty, open either the relative or absolute path directory, otherwise, open cwd
	if (!args.empty()) {
		// Get current working directory
		std::string cwd = DirectoryHelper::GetWorkingPath();

		// Error check if the received directory is valid
		if (cwd.empty()) {
			perror("An error occurred while GETTING the current directory");
			return;    // TODO: Do we need to exit a specific int if an error has occurred?
		}

		// Get the given argument directory at index 1
		const std::string &given_dir = args;

		if (!given_dir.empty() && given_dir.at(0) == '/') { // Absolute path
			desired_dir_string = given_dir;
		} else { // Relative path
			desired_dir_string = cwd + "/" + given_dir;
		}
	} else { // No argument given, just print cwd files
		desired_dir_string = "."; // . == cwd
	}
	desired_dir = opendir(desired_dir_string.c_str());

//	 If the directory couldn't be opened, print an error and return
	if (desired_dir == nullptr) {
		perror(("dir: cannot access '" + args + "'").c_str());  // Something happened while opening the directory
//		std::cout << "An error occurred while OPENING the directory \"" << desired_dir_string << "\"" << std::endl;
		return;
	}

	// The directory was successfully opened, create a directory entry object
	struct dirent *directory_info;

	// Use and reuse our newly created directory entry to output the name of every directory entry found.
	std::vector<std::string> entry_name_vect;
	while ((directory_info = readdir(desired_dir)) != nullptr)
		entry_name_vect.emplace_back(directory_info->d_name);

	// Sort the file names in the vector so we can have a nice alphabetical output
	std::sort(entry_name_vect.begin(), entry_name_vect.end());

	// Output every file name
	for (const std::string &entry_name: entry_name_vect)
		if (entry_name != "." && entry_name != "..")
			std::cout << entry_name << std::endl;

	// Cleanup.  Close the directory.
	closedir(desired_dir);
}