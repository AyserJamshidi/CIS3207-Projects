//
// Created by lmfaoown on 10/6/21.
//
#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include <string>

class InternalCommand {
public:
	/**
	 * Changes the current working directory to the desired directory given in args
	 *
	 * @param args The directory to change the shell's cwd to
	 */
	static void cd(const std::string &args);

	/**
	 * Clears the terminal window entirely
	 */
	static void clr();

	/**
	 * Outputs all entries for the desired directory given in args
	 *
	 * @param args The directory to output
	 */
	static void dir(const std::string &args);

	/**
	 * Outputs all available environment variables
	 */
	static void environ();

	/**
	 * Outputs the string message through stdout
	 *
	 * @param args The message to output
	 */
	static void echo(const std::string &args);

	/**
	 * Reads the file "readme_doc" in the shell's local directory with the more filter
	 */
	static void help();

	/**
	 * Keeps this shell paused until the user inputs the 'ENTER' key.
	 */
	static void pause();

	/**
	 * Overwrites the current PATH environment variable with all given directories in the args parameter.
	 *
	 * @param args Paths, split with a space, to overwrite the current PATH environment variable with.
	 */
	static void path(std::string args);
};
#endif