//
// Created by aceja on 9/2/2021.
//
#include <iostream> // I/O to console
#include <fstream> // File manipulating

#define CURR 1
#define PREV 0

void output(char letter, int char_counter);

int main(int argc, char **argv) {
	// Exit peacefully if no arguments have been given while outputting proper syntax.
	if (argc == 1) {
		std::cout << "wzip: file1 [file2 ...]" << std::endl;
		return 0;
	}

	// Loop variables
	int char_counter = 0;
	char letters[2];

	// Loop every given argument starting at 1, so we can ignore the file path
	for (int i = 1; i < argc; i++) {
		std::ifstream input_file(argv[i], std::ios_base::binary);

		// Ensure that we successfully loaded the desired file.
		if (!input_file.is_open()) {
			std::cout << "wzip: cannot open file" << std::endl;
			return 1;
		}

		// Loop every character in the file while storing the current char and the previous char
		while (letters[PREV] = letters[CURR], input_file.get(letters[CURR])) {

			// If we've encountered a new letter
			if (letters[PREV] != letters[CURR]) {
				// Output the current letter with the corresponding count then reset the count
				output(letters[PREV], char_counter);
				char_counter = 0;
			}

			char_counter++;
		}

		output(letters[PREV], char_counter);

		// Close the file handle
		input_file.close();

		// Reset variables
		letters[PREV] = letters[CURR] = char_counter = 0;
	}

	return 0;
}

void output(char letter, int char_counter) {
	// Skip the letter if it's null or "carriage return" char.  No idea why they appear but they do.
	if (letter == 0 || letter == 0xF)
		return;

	fwrite(&char_counter, 4, 1, stdout);
	fwrite(&letter, 1, 1, stdout);
}