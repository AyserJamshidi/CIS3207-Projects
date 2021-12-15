//
// Created by ayserjamshidi on 9/15/21.
//
#include <iostream>
#include "config_file.h"
#include <cstring>

bool config_file::isDebugging() const {
    return debug_mode;
}

bool config_file::LoadFile() {
    return LoadFile((char *) "config.txt");
}

bool config_file::LoadFile(char *input) {
    file.open(input);

    return file.is_open();
}

int config_file::GetValue(const std::string &desired_var) {
    std::string current_line;

    while (getline(file, current_line)) {
        std::size_t index = current_line.find(desired_var);

        if (index != std::string::npos) {
            current_line.erase(index, std::strlen(desired_var.c_str()) + 1); // + 1 to take care of the space after finding desired_var

            if (debug_mode)
                std::cout << "Set " << desired_var << " to " << current_line << std::endl;

            // Reset any flags and set the file to start at the beginning for the next time we read the config file.
            file.clear();
            file.seekg(0);
            return std::stoi(current_line);
        }
    }

    file.clear();
    file.seekg(0);
    return -1;
}