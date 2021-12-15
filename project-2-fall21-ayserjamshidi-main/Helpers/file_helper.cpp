//
// Created by lmfaoown on 10/22/21.
//

#include <string>
#include <sys/stat.h> // Stat for checking if file exists

#include "file_helper.h"

bool FileHelper::FileExists(const std::string &name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}