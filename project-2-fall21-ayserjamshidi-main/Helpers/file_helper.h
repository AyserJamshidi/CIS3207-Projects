//
// Created by lmfaoown on 10/22/21.
//

#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <string>

class FileHelper {
public:
	/**
	 * Determines if the given file name exists or not.
	 *
	 * @param name File name
	 * @return true if the file exists, false otherwise.
	 */
	static bool FileExists(const std::string &name);
};


#endif //FILE_HELPER_H
