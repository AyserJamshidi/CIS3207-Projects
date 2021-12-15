#ifndef VECTOR_MANIPULATION_H
#define VECTOR_MANIPULATION_H

#include <string>
#include <vector>

class VectorManipulation {
public:
	/**
	 * @brief Used for INTERACTIVE mode.
	 * Splits String str into multiple string entries into vector vect using the delimiter delim
	 *
	 * @param str Desired string to split
	 * @param vect Vector to store string entries into
	 * @param delim Delimiter to split by
	 */
	static void TokenizeStringInteractiveMode(const std::string& str, std::vector<std::string> &vect, char delim);

	/**
	 * @brief Used for BATCH mode
	 * Splits String str into multiple string entries into vector vect using the delimiter delim
	 *
	 * @param str Desired string to split
	 * @param vect Vector to store string entries into
	 * @param delim Delimiter to split by
	 */
	static void TokenizeStringBatchMode(const std::string &str, std::vector<std::string> &vect, char delim);

	/**
	 * @brief Searches for an exact string inside of the given vector
	 *
	 * @param element Exact string to search for
	 * @param vect Vector with string entries
	 *
	 * @return index of the found element or -1 if not found
	 */
	static int FindElementInVector(const std::string &element, std::vector<std::string> &vect);

	/**
	 * @brief Removes all trailing spaces in the head and tail of all strings in the given vector
	 *
	 * @param given_vector Vector to clean elements
	 */
	static void CleanTrailingSpaces(std::vector<std::string> &given_vector);

};

#endif // VECTOR_MANIPULATION_H
