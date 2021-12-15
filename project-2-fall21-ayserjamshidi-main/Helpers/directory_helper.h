//
// Created by lmfaoown on 10/6/21.
//
#include <string>

class DirectoryHelper {
public:
	/**
	 * @brief	Acquire the shell's current working directory.
	 *
	 * @return	The current working directory.
	 */
    static std::string GetWorkingPath();
};