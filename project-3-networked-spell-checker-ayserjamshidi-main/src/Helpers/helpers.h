//
// Created by ayserjamshidi on 11/2/21.
//

#ifndef PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_HELPERS_H
#define PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_HELPERS_H

#include <vector>
#include <string>
#include <unordered_map>

class Helpers {

public:
	class MapManipulation {
	public:
		/**
		 * Splits a given string into a LEFT:RIGHT KEY:VALUE pair where
		 * LEFT is the key, : is the delimiter and RIGHT is the value.
		 *
		 * @param str String to turn into a hashmap
		 * @param map The hashmap to modify
		 * @param delim Delimiter symbol
		 */
		static void CreateHashMap(const std::string& str, std::unordered_map<std::string, std::string> &map, char delim);
	};

	class Print {
	public:
		/**
		 * Outputs a hash table in a legible format.
		 * @param m Hash table
		 */
		static void HashTable(const std::unordered_map<std::string, std::string> &m);
	};

	class Generics {
	public:
		/**
		 * Generates a random number inside a given range
		 *
		 * @param min minimum number to generate
		 * @param max maximum number to generate
		 * @return A generated number between [min, max]
		 */
		static int rng(int min, int max);

		/**
		 * Determines if a given string is only numbers
		 *
		 * @param s string to check
		 * @return true if the string is a string-integer, false otherwise
		 */
		static bool is_integer(const std::string &s);
	};
};

#endif //PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_HELPERS_H
