//
// Created by Ace on 9/4/2021.
//
#include <iostream> // I/O to console
#include <fstream> // File manipulating

int main(int argc, char **argv) {
	// Exit peacefully if no arguments have been given while outputting proper syntax.
	if (argc == 1) {
		std::cout << "wunzip: file1 [file2 ...]" << std::endl;
		return 0;
	}

	// Loop every given file
	for (int i = 1; i < argc; i++) {
		std::ifstream input_file(argv[i], std::ios_base::binary);

		// Ensure that we successfully loaded the desired file.
		if (!input_file.is_open()) {
			std::cout << "wunzip: cannot open file" << std::endl;
			return 1;
		}

		// Storage variables
		char letter;
		char *num = new char[4];

		// Loop every 5 bytes and print it out until we run out of bytes
		while (input_file.read(num, 4), input_file.read(&letter, 1))
			for (int j = 0; j < *num; j++)
				printf("%c", letter);

		// Close the current file
		input_file.close();
	}

	return 0;
}