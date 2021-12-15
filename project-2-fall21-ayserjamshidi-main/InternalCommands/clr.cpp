//
// Created by lmfaoown on 10/6/21.
//
#include <iostream>

#include "internal_commands.h"

void InternalCommand::clr() {
	// Doesn't work on Temple Servers
//	std::cout << R"(\e[H\e[2J\e[3J)" << std::endl;
//	std::cout << "\\e[1;1H\\e[2J" << std::endl;

	std::cout << "\033c"; // Works on Temple Servers
}