//
// Created by lmfaoown on 10/21/21.
//
#include <algorithm>
#include "internal_commands.h"
#include "../Helpers/vector_manipulation.h"

void InternalCommand::path(std::string args) {
	// From the beginning to the end of args, replace all empty spaces with ':'
	std::replace(args.begin(), args.end(), ' ', ':');

	// Set and overwrite the PATH environment value to the args c-string equivalent
	setenv("PATH", args.c_str(), 1);
}