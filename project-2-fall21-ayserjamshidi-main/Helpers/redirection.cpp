//
// Created by lmfaoown on 10/7/21.
//

#include <unistd.h>
#include <iostream>
#include <fcntl.h>

#include "redirection.h"

int Redirection::InitializeInputRedirect(std::string &input_file) {
	// Save the current STDIN FD.
	int stdin_save = dup(STDIN_FILENO);

	// Create a new STDIN FD with the desired input file.
	int new_fd = open(input_file.c_str(), O_RDONLY);

	// Create redirection by overwriting our current STDIN with the input file's.
	dup2(new_fd, STDIN_FILENO);
	close(new_fd);

	// Return the overwritten STDIN FD so we can restore it later.
	return stdin_save;
}

int Redirection::InitializeOutputRedirect(int output_mode, std::string &output_file) {
	// Ensures the user isn't using more than one or two > symbols
	if (output_mode < 1 || 2 < output_mode) {
		std::cout << "You can only use > or >> as an output mode." << std::endl;
		return -1;
	}

	// Save the current STDOUT FD.
	int stdout_save = dup(STDOUT_FILENO);

	/*
	 * Create a new STDOUT FD with the desired output file.
	 *
	 * We append the following for the corresponding reasons:
	 *      O_CREAT to create the output file if it doesn't exist.
	 *      O_WRONLY for write permissions as reading is unnecessary.
	 *
	 *      O_TRUNC if output_mode == 1 to overwrite the output file completely.    (>)
	 *      O_APPEND if output_mode != 1 to append to the output file.              (>>)
	 */
	int new_fd = open(output_file.c_str(), O_CREAT | O_WRONLY | (output_mode == 1 ? O_TRUNC : O_APPEND));

	// Create redirection by overwriting our current STDOUT with the output file's.
	dup2(new_fd, STDOUT_FILENO);
	close(new_fd);

	// Return the overwritten STDOUT FD so we can restore it later.
	return stdout_save;
}

void Redirection::TerminateRedirect(int redirect_restore, int fileno) {
	// Flush stdin if we're terminating an STDIN_FILENO redirect, otherwise flush stdout.
	fflush(fileno == 0 ? stdin : stdout);

	// Overwrite the given fileno descriptor with the given redirect_restore.
	dup2(redirect_restore, fileno);
	close(redirect_restore);
}