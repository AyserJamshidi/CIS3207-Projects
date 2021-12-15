//
// Created by ayserjamshidi on 10/29/21.
//

#ifndef PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_DICTIONARY_H
#define PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_DICTIONARY_H

#include <fstream> // file streams for reading dictionary
#include <unordered_map> // hash tables

class dictionary {
public:
	/**
	 * Loads all of the words inside of the given file into the dictionary hash table.
	 *
	 * @param input desired dictionary file.
	 * @return The amount of words loaded.
	 */
	int LoadFile(char *input);

	/**
	 * Checks if the dictionary contains the given word.
	 *
	 * @param word word to spellcheck.
	 * @return "OK" if the dictionary contains the word, "MISSPELLED" otherwise.
	 */
	const char *SpellCheck(const char *word);
};


#endif //PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_DICTIONARY_H
