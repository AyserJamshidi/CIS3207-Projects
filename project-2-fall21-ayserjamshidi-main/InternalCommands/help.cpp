//
// Created by lmfaoown on 10/7/21.
//
#include <fstream>
#include <iostream>
#include "internal_commands.h"

void InternalCommand::help() {
	// Read the manual
	std::ifstream manual_file("readme_doc");

	// Ensure the manual opened successfully and output proper error messages if not.
	if (!manual_file.is_open()) {
		perror("An error occurred while opening the help manual");
		return;
	}

	// Output every line found in the manual file
	std::string current_line;
	while (getline(manual_file, current_line))
		std::cout << current_line << std::endl;

	// Close the file to prevent any memory leakage and such
	manual_file.close();
}
