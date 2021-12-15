//
// Created by aceja on 8/26/2021.
//
#include <iostream> // I/O to console
#include <fstream> // File manipulating

int main(int argc, char **argv) {
    // Exit peacefully if no arguments have been given.
    if (argc == 1)
        return 0;

    std::string current_line;

    // Loop every given argument starting at 1 so we can ignore the file path
    for (int i = 1; i < argc; i++) {
        std::ifstream input_file(argv[i]);

        // Ensure that we successfully loaded the desired file.
        if (!input_file.is_open()) {
            std::cout << "wcat: cannot open file" << std::endl;
            return 1;
        }

        // Output every current_line that contains
        while (getline(input_file, current_line))
            std::cout << current_line << std::endl;

        // Close the file handle
        input_file.close();
    }

    return 0;
}