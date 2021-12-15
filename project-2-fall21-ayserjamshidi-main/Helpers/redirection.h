//
// Created by lmfaoown on 10/7/21.
//
#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <string>

class Redirection {
public:
	/**
	 * @brief Initializes an input redirection with the given input file.
	 *
	 * @param input_file Desired file to use as input.
	 * @return FD that was overwritten.
	 */
	static int InitializeInputRedirect(std::string &input_file);

	/**
	 * @brief Initializes an output redirection with the given output file.
	 *
	 * @param output_mode 1 = Truncate (>), 2 = Append (>>)
	 * @param output_file Desired file to output onto.
	 * @return FD that was overwritten.
	 */
	static int InitializeOutputRedirect(int output_mode, std::string &output_file);

	/**
	 * @brief Overwrites a redirection with redirect_restore in respect to the given fileno
	 *
	 * @param redirect_restore Descriptor to overwrite into the fileno position
	 * @param fileno Position to overwrite (0 or 1)
	 */
	static void TerminateRedirect(int redirect_restore, int fileno);
};

#endif