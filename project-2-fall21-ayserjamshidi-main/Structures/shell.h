//
// Created by lmfaoown on 10/7/21.
//
#ifndef SHELL_H
#define SHELL_H

#include <string>

/**
 * Shell struct that makes it easy for functions to announce the shell's name and suffix
 */
struct shell {
	std::string name = "myshell";
	std::string name_suffix = ">";
};

#endif
