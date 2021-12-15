#include <iostream> // i/o
#include <unistd.h> // fork/exec/dup/dup2
#include <algorithm> // find
#include <sys/wait.h> // pidwait
#include <fstream> // File reading for batch mode

#include <string> // string
#include <vector> // arrays
#include <map> // maps

#include "Structures/shell.h"
#include "InternalCommands/internal_commands.h"
#include "Helpers/directory_helper.h"
#include "Helpers/vector_manipulation.h"
#include "Helpers/redirection.h"
#include "Helpers/file_helper.h"
#include "Helpers/print_helper.h"

/**
 * @brief Runs a given command with the desired arguments with or without being ran in the background.
 *
 * @param command Desired command to run.
 * @param args Arguments for desired command.
 * @param background True/False if the program should run in the background.
 */
void RunCommand(const std::string &command, const std::string &args, int background) {
	pid_t pid = -1; // PID container for when we fork

	/**
	 * If we want to run in the background, create a child and allow only the child to
	 * progress while ensuring the parent stops here and outputting errors, if any.
	 */
	if (background) {
		pid = fork();

		if (pid != 0) { // We're not the child, let's return!
			if (pid < 0) // We've encountered an error while forking.  Output an error!
				perror(std::string("An error occurred while forking for the command '" + command + "'").c_str());

			return;
		}
	}

	if (command == "cd") { // CD Internal command
		InternalCommand::cd(args);
	} else if (command == "clr") { // CLR Internal command
		InternalCommand::clr();
	} else if (command == "dir") { // DIR Internal command
		InternalCommand::dir(args);
	} else if (command == "environ") { // ENVIRON Internal command
		InternalCommand::environ();
	} else if (command == "path") { // PATH Internal command
		InternalCommand::path(args);
	} else if (command == "echo") { // ECHO Internal command
		InternalCommand::echo(args);
	} else if (command == "help") { // TODO: NEEDS TO USE MORE FILTER
		InternalCommand::help();
	} else if (command == "pause") { // PAUSE Internal command
		InternalCommand::pause();
	} else if (command == "quit") { // QUIT Internal command
		exit(0);
	} else {
		pid = fork(); // Create a child to ensure the parent shell doesn't crash

		if (pid < 0) {
			perror(std::string("An error occurred while forking for the command '" + command + "'").c_str());
		} else if (pid == 0) { // Child
			// C-string array as execvp must have C-strings
			std::vector<char *> container_arr;

			// Add the command name as the first argument
			container_arr.push_back((char *) command.c_str());

			// Get all arguments into a vector string and place each string object into a vector c-string array with conversion
			std::vector<std::string> argument_arr; // Will contain exclusively args in string format
			VectorManipulation::TokenizeStringInteractiveMode(args, argument_arr, ' ');

			// Store all the vector elements into a char* array
			for (const std::string &cur_arg: argument_arr)
				container_arr.push_back((char *) cur_arg.c_str());

			container_arr.push_back(nullptr); // Add a null terminator at the end of the array

			// Run the command, spit out any errors and exit the child)
			if (execvp(command.c_str(), &container_arr[0]) == -1) {
				perror(std::string("An error occurred while attempting to execute '" + command + "'").c_str());
			}
			exit(1); // Just in case the program doesn't terminate correctly
		} else { // Parent
			// If this command isn't supposed to be ran in the background then wait for the child to terminate
			if (!background)
				waitpid(pid, nullptr, 0);
		}
	}

	// Exit the background child after it's done executing
	if (pid == 0)
		exit(0);
}

/**
 * @brief Prepares the shell for any redirection and then executes the interpreted command through RunCommand();
 *
 * @param interpreted_cmds Reference map of interpreted commands
 */
void ExecuteInterpretedCommand(std::map<std::string, std::string> &interpreted_cmds) {
	/**
	 * Map.count returns 1 or 0 so we use it as a boolean initially
	 *
	 * It will then be overwritten inside of their respective if-statement scope when duping
	 * for future restoration.
	 */
	int input_redirect = interpreted_cmds.count("STDIN_REDIRECT_INIT");
	int output_redirect = interpreted_cmds.count("STDOUT_REDIRECT_INIT");

	// If we found an input redirect symbol
	if (input_redirect) {
		// Get the filename we're redirecting into our command.
		std::string input_file = interpreted_cmds.at("STDIN_REDIRECT_INIT");

		// Ensure the input file exists.  If it doesn't, spit out an error and stop executing this command.
		if (!FileHelper::FileExists(input_file)) {
			std::cout << "Input file '" << input_file << "' could not be found." << std::endl;
			return;
		}

		// Overwrite input_redirect with the file descriptor of our current stdin
		input_redirect = Redirection::InitializeInputRedirect(input_file);
	}

	// If we found an output redirect symbol
	if (output_redirect) {
		// Get the filename we're outputting into.
		std::string output_file = interpreted_cmds.at("STDOUT_REDIRECT_INIT");

		/**
		 * Convert the first character of output_file as during interpretation we set the first character to 1 or 2
		 * 		1 = > 	= Truncating output to file
		 * 		2 = >> 	= Appending output to file
		 */
		int input_mode = std::stoi(std::string(1, output_file.at(0)));

		// Remove the first character now that we're done dealing with it
		output_file.erase(0, 1);

		// Overwrite output_redirect with the file descriptor of our current stdout
		output_redirect = Redirection::InitializeOutputRedirect(input_mode, output_file);
	}

	// Execute command
	RunCommand(interpreted_cmds.at("CMD"), interpreted_cmds.at("ARGS"), interpreted_cmds.count("BG"));

	// Restore original file descriptor if we overwrote anything
	if (input_redirect)
		Redirection::TerminateRedirect(input_redirect, STDIN_FILENO);

	if (output_redirect)
		Redirection::TerminateRedirect(output_redirect, STDOUT_FILENO);
}

/**
 * @brief Checks the given vector of string tokens for the given symbol.  If the redirection symbol was found then
 * add the respective key and value pair into the given map.
 *
 * @param symbol Redirection symbol to check for
 * @param cmd_tokens Vector of tokens to search through
 * @param cmd_interpretation Interpretation map to input new key-value pairs into
 * @return
 */
bool CheckAndInterpretRedirect(const std::string &symbol, std::vector<std::string> &cmd_tokens, std::map<std::string, std::string> &cmd_interpretation) {
	signed int tok_position = VectorManipulation::FindElementInVector(symbol, cmd_tokens);

	// If the element was found in the vector
	if (tok_position != -1) {
		// Ensure we have a token to the right of the symbol
		if ((signed int)cmd_tokens.size() <= tok_position + 1) {
			std::cout << "You must have a token to the right of " << symbol << std::endl;
			return false; // Return false so the shell knows to skip this command due to improper syntax with redirection symbols
		}

		// If we're doing an output redirection, append the output mode (1 or 2) so we can make it the first character of the interpreted output value
		std::string stdout_inclusion;
		if (symbol == ">")
			stdout_inclusion.append("1");
		else if (symbol == ">>")
			stdout_inclusion.append("2");

		// Add stdout_inclusion as the first character and create the respective key for what redirection we're diong
		cmd_interpretation.emplace(symbol == "<" ? "STDIN_REDIRECT_INIT" : "STDOUT_REDIRECT_INIT", stdout_inclusion + cmd_tokens[tok_position + 1]);

		// Erase < and 1 token to the right of it
		cmd_tokens.erase(cmd_tokens.begin() + tok_position, cmd_tokens.begin() + tok_position + 2);
	}

	return true;
}

/**
 *
 *
 * @param cmd_tokens
 */
void InterpretCommand(std::vector<std::string> cmd_tokens) {
	std::map<std::string, std::string> interpretation;

	// Background
	std::string &last_token = cmd_tokens[cmd_tokens.size() - 1];
	int last_char_pos = last_token.size() - 1;

	if (last_token.at(last_char_pos) == '&') {
		interpretation.emplace("BG", "true");
		last_token.erase(last_char_pos, 1);
	}

	// Handle all redirection symbols and return if any of them return false
	if (!CheckAndInterpretRedirect("<", cmd_tokens, interpretation)
	    || !CheckAndInterpretRedirect(">", cmd_tokens, interpretation)
	    || !CheckAndInterpretRedirect(">>", cmd_tokens, interpretation))
		return;

	interpretation.emplace("CMD", cmd_tokens[0]);
	cmd_tokens.erase(cmd_tokens.begin());

	std::string leftover_args;

	// Remaining tokens SHOULD be args for the initial command.  Ensure this is right!!!
	for (unsigned int i = 0; i < cmd_tokens.size(); i++) {
		leftover_args.append(cmd_tokens[i]);

		if (i != ((unsigned int)cmd_tokens.size() - 1))
			leftover_args.append(" ");
	}

	interpretation.emplace("ARGS", leftover_args);

	ExecuteInterpretedCommand(interpretation);
}

void EnsureAndTokenize(std::vector<std::string> &command_lines) {
	const char redirection_symbols[] = {'<', '>', '|', '&'};

	for (std::string &cur_command: command_lines) { // Loop through every command line
		// Ensure cur_command doesn't attempt to try things before actually giving us commands
		for (char symbol: redirection_symbols)
			if (cur_command.at(0) == symbol) {
				std::cout << "You must have an initial parameter before using redirection or pipes." << std::endl;
				return;
			}

		// Tokenize
		std::vector<std::string> tokens;
		VectorManipulation::TokenizeStringInteractiveMode(cur_command, tokens, ' ');

		InterpretCommand(tokens);
	}
}

/**
 * @brief Continuously asks the user for a line of commands until it is <= 80 in length and is not just white space, storing it into str
 *
 * @param given_shell Shell for printing purposes
 * @param str String to store the user's input
 * @return
 */
std::string GetInput(const shell &given_shell, std::string &str) {
	while (true) {
		std::getline(std::cin, str);

		// If the input is acceptable, proceed.
		if (str.length() <= 80) { // If the received string is an acceptable length, ensure it has actual commands instead of just white space.
			// We ensure the command isn't just useless white spaces as that'll crash the shell when interpreting.
			bool acceptable_input = false;

			for (char cur_letter: str)
				if (cur_letter != ' ')
					acceptable_input = true;

			if (acceptable_input)
				break;
		} else
			std::cout << "Too many characters.  Use 80 or less." << std::endl;

		PrintHelper::PrintShellText(given_shell);
	}

	return str;
}

int main(int argc, char **argv) {
	// Reused variables
	shell my_shell;

	if (argc == 1) {  // Interactive mode
		while (true) { // We can infinitely loop as the quit command will call exit() to terminate the shell
			std::vector<std::string> split_commands;

			PrintHelper::PrintShellText(my_shell);

			// Request user input and store it all
			std::string given_input;
			GetInput(my_shell, given_input);

			// Split user input by & to indicate individual commands
			VectorManipulation::TokenizeStringInteractiveMode(given_input, split_commands, '&');

			// Erase head and tail trailing spaces as this is a new command to assist in interpreting the command later
			VectorManipulation::CleanTrailingSpaces(split_commands);

			// Add & symbol to all tokens except for the last one to let the interpreter in the future know it's a background task
			for (signed int i = 0; i < ((signed int) split_commands.size()) - 1; i++)
				split_commands[i].append("&");

			// If the split commands is somehow empty then just re-loop
			if (split_commands.empty())
				continue;

			// Ensures that we don't have a redirection or something similar as the first token, then runs the command
			EnsureAndTokenize(split_commands);
		}
	} else if (argc == 2) { // Batch mode
		std::vector<std::string> split_commands;
		std::ifstream given_file(argv[1]);

		if (!given_file.is_open()) {
			std::cout << "The file '" << argv[1] << "' could not be found." << std::endl;
			exit(0);
		}

		std::string received_input;
		while (std::getline(given_file, received_input))
			VectorManipulation::TokenizeStringBatchMode(received_input, split_commands, '&');

		// Erase head and tail trailing spaces as this is a new command to assist in interpreting the command later
		VectorManipulation::CleanTrailingSpaces(split_commands);

		if (split_commands.empty()) {
			std::cout << "No commands could be found." << std::endl;
			return 0;
		}

		// Ensures that we don't have a redirection or something similar as the first token, then runs the command
		EnsureAndTokenize(split_commands);
	} else {
		std::cout << "ERROR: You can only have a single argument when calling this shell." << std::endl;
	}
}