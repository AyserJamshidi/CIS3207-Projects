//
// Created by ayserjamshidi on 10/23/21.
//

#ifndef PRINT_HELPER_H
#define PRINT_HELPER_H

#include <string>
#include <map>
#include <vector>

#include "../Structures/shell.h"

class PrintHelper {
public:
	/**
	 * @brief Debug function that will print out all elements in a vector
	 */
	static void PrintVector(std::vector<std::string> &vect);

	/**
	 * @brief Debug function that will print out all key-value pairs in a map
	 */
	template<typename K, typename V>
	static void PrintMap(const std::map<K, V> &m);

	/**
	 * @brief Prints out the current working directory with given_shell's name and suffix
	 *
	 * @param given_shell Desired shell to output
	 */
	static void PrintShellText(const shell &given_shell);
};


#endif //PRINT_HELPER_H
