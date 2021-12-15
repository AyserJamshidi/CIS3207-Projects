//
// Created by ayserjamshidi on 9/15/21.
//
#include <fstream>

class config_file {
private:
    bool debug_mode;
	std::ifstream file;

public:
	/**
	 * @return true if this object has been set to debug mode, false otherwise.
	 */
	bool isDebugging() const;

	/**
	 * Attempts to load config.txt (the default config filename) into fstream.
	 *
	 * @param input
	 * @return true if the file was successfully opened, false otherwise.
	 */
	bool LoadFile();

    /**
     * Attempts to load the given input (filename) into fstream.
     *
     * @param input
     * @return true if the file was successfully opened, false otherwise.
     */
	bool LoadFile(char *input);

	/**
	 * Attempts to retrieve a value from config_file by searching for {@code desired_var}
	 *
	 * @param desired_var
	 * @return The integer value assigned that succeeds the found
	 */
	int GetValue(const std::string& desired_var);

};
