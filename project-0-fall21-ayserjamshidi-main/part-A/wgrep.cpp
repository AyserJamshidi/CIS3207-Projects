//
// Created by aceja on 8/26/2021.
//
#include <iostream> // I/O to console
#include <fstream> // File manipulating
#include <cstring> // strstr()

int main(int argc, const char **argv) {
    if (argc == 1) { // ! Not enough arguments
        std::cout << "wgrep: searchterm [file ...]" << std::endl;
        return 1;
    }

    // Peacefully exits wgrep if we have an empty search term as it'll be useless to proceed
    if (strlen(argv[1]) == 0)
        return 0;

    std::string current_line;

    if (argc == 2) { // ! wgrep the user's stdin
        // Endlessly loop until the user terminate the program with Ctrl+C
       while (std::getline(std::cin, current_line))
            if (strstr(current_line.c_str(), argv[1]))
                std::cout << current_line << std::endl;
    } else { // ! Use arguments to do everything automatically
        // Loop every given argument starting at 2, so we can ignore the file path and the search term
        for (int i = 2; i < argc; i++) {
            // Open file
            std::ifstream input_file(argv[i]);

            // Ensure that we successfully loaded the desired file.
            if (!input_file.is_open()) {
                std::cout << "wgrep: cannot open file" << std::endl;
                return 1;
            }

            // Output every line that contains argv[1] (the search terms)
            while (getline(input_file, current_line))
                if (strstr(current_line.c_str(), argv[1]))
                    std::cout << current_line << std::endl;

            // Close the file handle
            input_file.close();
        }
    }

    return 0;
}