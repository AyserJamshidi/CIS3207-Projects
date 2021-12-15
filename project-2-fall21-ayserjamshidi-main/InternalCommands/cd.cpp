//
// Created by lmfaoown on 10/6/21.
//
#include <iostream>
#include <unistd.h>

#include "../Helpers/directory_helper.h"
#include "internal_commands.h"
#include "../Helpers/vector_manipulation.h"

void InternalCommand::cd(const std::string &args) {
	std::vector<std::string> tokens;
	VectorManipulation::TokenizeStringInteractiveMode(args, tokens, ' ');

	// Error check
	if (tokens.size() > 1) {
		std::cout << "You can't have more than one argument." << std::endl;
		return;
	}

	std::string cwd = DirectoryHelper::GetWorkingPath();

	// Error check if the received directory is valid
	if (cwd.empty()) {
		perror("An error occurred while GETTING the current directory");
		return;    // TODO: Do we need to exit a specific int if an error has occurred?
	}

	if (tokens.empty()) { // No argument is given.  Return current directory
		std::cout << cwd << std::endl;
	} else {
		const std::string &given_dir = args;
		std::string desired_dir_string;

		if (given_dir.empty() || given_dir == ".") { // Do nothing because we are literally in this directory!
			return;
		} else if (given_dir == "..") {
			desired_dir_string = given_dir;
		} else { // Given a real directory!
			if (given_dir.at(0) == '/') { // Absolute path
				desired_dir_string = given_dir;
			} else { // Relative path
				desired_dir_string = cwd + given_dir;
			}
		}

		// If != 0, then an error occurred!  Stop here to prevent changing PWD
		if (chdir(desired_dir_string.c_str()) != 0) {
			std::string err_output = "An error occurred while changing cwd to '" + desired_dir_string + "'";
			perror(err_output.c_str());
			return;
		}

		setenv("PWD", desired_dir_string.c_str(), true);
	}
}