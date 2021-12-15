//
// Created by lmfaoown on 10/6/21.
//
#include <iostream>

#include "internal_commands.h"

void InternalCommand::echo(const std::string &args) {
	std::cout << args << std::endl;
}