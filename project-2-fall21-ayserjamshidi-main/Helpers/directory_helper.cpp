//
// Created by lmfaoown on 10/6/21.
//
#include <sys/param.h>  // MAXPATHLEN
#include <unistd.h>     // getcwd

#include "directory_helper.h"

std::string DirectoryHelper::GetWorkingPath() {
	// Container for cwd
	char temp[MAXPATHLEN];

	// Output will become the current working directory, or empty if it couldn't be acquired.
	std::string output = (getcwd(temp, sizeof(temp)) ? std::string(temp) : "");

	// Append a '/' char at the end of the directory if we're not at the drive's root directory /
	if (output.size() > 1)
		output.append("/");

	// Returns the received current directory path
	return output;
}