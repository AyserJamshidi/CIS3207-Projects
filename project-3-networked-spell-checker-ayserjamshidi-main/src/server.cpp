//
// Created by ayserjamshidi on 10/28/21.
//
#include <pthread.h> // threading
#include <iostream> // input/output functions
#include <sys/socket.h> // accept()
#include <cstring> // bzero()
#include <unistd.h> // read()
#include <queue> // queues
#include <chrono> // system_clock class

#include "Helpers/open_listener_socket.h"
#include "Helpers/helpers.h"
#include "Headers/dictionary.h"
#include "Headers/errors.h"
#include "Headers/server_structures.h"

// Settings
const char *LOG_FILENAME = "spellcheck_server_log.txt";
std::string DICTIONARY = "words";
unsigned int PORT = 14567;
unsigned int MAX_WORKER_THREADS = 4;
unsigned int MAX_WORKER_QUEUE_SIZE = 20;
unsigned int MAX_MSG_LENGTH = 80;
unsigned int PRIORITY_MODE = 0;
bool CAN_LINEBREAK = false;

// Thread variables
pthread_mutex_t worker_mutex; // A worker thread mutex
pthread_mutex_t logger_mutex; // A logger thread mutex

// Thread conditions
pthread_cond_t client_added; // A thread has added a client socket into worker_queue.
pthread_cond_t client_removed; // A thread has removed a client socket from worker_queue.
pthread_cond_t logger_added; // A thread has added a logger_entry to logger_queue.
pthread_cond_t logger_removed; // A thread has removed a logger_entry to logger_queue.

// Queues
std::priority_queue<worker_entry, std::vector<worker_entry>, compare> *worker_queue;
//std::queue<int> *worker_queue;
std::queue<logger_entry> *logger_queue;

// Log output file stream
std::ofstream log_file;

// Dictionary
dictionary *dict;

// Worker-specific global variables
char removable_chars[] = {'\n', '\t', '\r', ' '};

/**
 * A worker thread.
 *
 * This thread is tasked to query and respond (with an OK or MISSPELLED)
 * any queued clients inside worker_queue until they quit.
 */
void *WorkerThread(void *arg) {
	char buff[MAX_MSG_LENGTH];
	int current_client_socket;

	while (true) {
		pthread_mutex_lock(&worker_mutex); // Force other threads to wait

		// Conditionally wait while the worker queue is client_removed for client_added to be signalled
		while (worker_queue->empty()) {
			pthread_cond_wait(&client_added, &worker_mutex);
		}

		// Get the current socket for this thread
		current_client_socket = worker_queue->top().socket;
		worker_queue->pop();
		pthread_cond_signal(&client_removed);
		pthread_mutex_unlock(&worker_mutex);

		std::cout << "Ready for data from client " << current_client_socket << std::endl;

		ssize_t length;
		while ((length = read(current_client_socket, buff, sizeof(buff))) > 0) {
			// Create a new logger_entry
			logger_entry current_entry;
			current_entry.arrival_time = clock();

			// If the buffer is somehow empty, just skip this
			if (length == 0)
				continue;

			// Overwrite the last unwanted symbol with a null terminator to properly look up the word in the dictionary
			for (const char &cur_char: removable_chars)
				if (buff[length - 1] == cur_char) {
					buff[length - 1] = '\0';
					break;
				}

			// Do another length after possibly modifying the buffer,
			if (strlen(buff) == 0)
				continue;

			std::cout << current_client_socket << " - RECV - " << buff << std::endl;

			// Get the spellcheck answer and store it
			const char *answer = dict->SpellCheck(buff);
			current_entry.completed_time = clock();

			// Send out the message to the client
			write(current_client_socket, answer, strlen(answer));

			// Double line break for neater output for human clients if this debug mode is on
			if (CAN_LINEBREAK)
				write(current_client_socket, "\n\n", 2);

			// Store the word and the result into the logger entry
			current_entry.word = buff;
			current_entry.result = answer;

			// Put the new log into the logger queue
			logger_queue->emplace(current_entry);
			pthread_cond_signal(&logger_added);

			// Zero-out for the next message, so we don't contain remnants of previous messages
			bzero(buff, MAX_MSG_LENGTH);
		}

		std::cout << "Client " << current_client_socket << " quit!" << std::endl;
	}
}

void log_to_file(const std::string &message) {
	// Open the log file
	log_file.open(LOG_FILENAME, std::ios_base::app);

	// Output to the log file
	log_file << message << std::endl;

	// Close and write to log file
	log_file.close();
}

std::string get_current_time() {
	// Do some time C++11 voodoo
	auto start_time = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(start_time);

	// Store the time in a human readable format
	std::string string_container = std::ctime(&time);

	// Remove the last character in the string container as it's a line break
	string_container.pop_back();

	return string_container;
}

/**
 * A logger thread
 *
 * This thread is tasked to query logger_queue and output the word
 * and its results to LOG_FILENAME.
 * @param arg
 * @return
 */
void *LoggerThread(void *arg) {
	// * Start log file check
	// Ensure we're able to open the log file.  If we can, just close it for now so we can log stuff later.
	log_file.open(LOG_FILENAME, std::ios_base::app);
	if (!log_file.is_open()) {
		perror("Logger thread failed to create the log file");
		exit(LOGFILE_CREATE_FAILURE);
	}
	log_file.close();
	// * End log file check
	log_to_file("A new session has started at " + get_current_time());

	logger_entry current_logger_entry;
	while (true) {
		pthread_mutex_lock(&logger_mutex);

		while (logger_queue->empty()) {
			pthread_cond_wait(&logger_added, &logger_mutex);
		}

		// Get the next log entry
		current_logger_entry = logger_queue->front();
		logger_queue->pop();
		pthread_cond_signal(&logger_removed);

		// Output to file
		log_to_file("Text: '" + current_logger_entry.word
		            + "' -- Response: '" + current_logger_entry.result
		            + "' -- Time for response: " + std::to_string(float(current_logger_entry.completed_time - current_logger_entry.arrival_time)) + "ms");

		// Unlock this thread's mutex
		pthread_mutex_unlock(&logger_mutex);
	}
}

int main(int argc, const char *argv[]) {
	// * Start initialize
	// cond/worker_mutex
	pthread_cond_init(&client_added, nullptr);
	pthread_cond_init(&client_removed, nullptr);

	pthread_cond_init(&logger_added, nullptr);
	pthread_cond_init(&logger_removed, nullptr);

	pthread_mutex_init(&worker_mutex, nullptr);
	pthread_mutex_init(&logger_mutex, nullptr);

	// queues
	worker_queue = new std::priority_queue<worker_entry, std::vector<worker_entry>, compare>;
	logger_queue = new std::queue<logger_entry>;
	// * End initialization

	// * Start arguments
	if (argc > 1) {
		// Apply all c-strings inside of argv to be their own individual token in the string vector
		std::unordered_map<std::string, std::string> parsed_arguments;

		// Parse the arguments by splitting ever '=' symbol, starting at 1 to skip the binary path at argv[0]
		for (int i = 1; i < argc; i++)
			Helpers::MapManipulation::CreateHashMap(argv[i], parsed_arguments, '=');

		// If we have a port key, assign the key's value to PORT
		if (parsed_arguments.count("-port")) {
			std::string input = parsed_arguments.at("-port");
			std::string err_msg = "-port must contain integers in the range of 1 to 65535.";

			// Ensure this input is nothing but numbers
			if (!Helpers::Generics::is_integer(input)) {
				std::cout << err_msg << std::endl;
				exit(PORT_NOT_NUMBER);
			}

			PORT = std::stoi(parsed_arguments.at("-port"));

			// Ensure this number is within the desired range
			if (!(1 <= PORT && PORT <= 65535)) {
				std::cout << err_msg << std::endl;
				exit(PORT_OUT_OF_RANGE);
			}

			std::cout << "Port has been set to '" << PORT << "'" << std::endl;
		}

		// If we have a dict key, assign the key's value to DICTIONARY
		if (parsed_arguments.count("-dict")) {
			DICTIONARY = std::string(parsed_arguments.at("-dict"));
			std::cout << "Dictionary has been set to '" << DICTIONARY << "'" << std::endl;
		}

		if (parsed_arguments.count("-workers")) {
			std::string input = parsed_arguments.at("-workers");
			std::string err_msg = "-workers must be an integer and is at least 1.";

			if (!Helpers::Generics::is_integer(input)) {
				std::cout << err_msg << std::endl;
				exit(WORKERCOUNT_NOT_NUMBER);
			}

			MAX_WORKER_THREADS = std::stoi(input);

			if (MAX_WORKER_THREADS < 1) {
				std::cout << err_msg << std::endl;
				exit(WORKERCOUNT_OUT_OF_RANGE);
			}

			std::cout << "Max worker threads has been set to '" << MAX_WORKER_THREADS << "'" << std::endl;
		}

		if (parsed_arguments.count("-max_msg_length")) {
			std::string input = parsed_arguments.at("-msg_length");
			std::string err_msg = "-max_msg_length must be an integer and is at least 1.";

			if (!Helpers::Generics::is_integer(input)) {
				std::cout << err_msg << std::endl;
				exit(MAX_MSG_LENGTH_NOT_NUMBER);
			}

			MAX_MSG_LENGTH = std::stoi(parsed_arguments.at("-msg_length"));

			if (MAX_MSG_LENGTH < 1) {
				std::cout << err_msg << std::endl;
				exit(MAX_MSG_LENGTH_OUT_OF_RANGE);
			}

			std::cout << "Max message length has been set to " << MAX_MSG_LENGTH << std::endl;
		}

		if (parsed_arguments.count("-mode")) {
			std::string input = parsed_arguments.at("-mode");
			std::string err_msg = "-mode must be an integer.";

			if (!Helpers::Generics::is_integer(input)) {
				std::cout << err_msg << std::endl;
				exit(MODE_NOT_NUMBER);
			}

			PRIORITY_MODE = std::stoi(parsed_arguments.at("-mode"));

			std::cout << "Priority mode has been turned " << (PRIORITY_MODE == 0 ? "off." : "on.") << std::endl;
		}

		if (parsed_arguments.count("-linebreak")) {
			CAN_LINEBREAK = parsed_arguments.at("-linebreak") == "true";

			std::cout << "Debug linebreaking mode has been turned on." << std::endl;
		}
	}
	// * End arguments

	// * Start dictionary
	std::cout << "Loading Dictionary '" << DICTIONARY << "'... ";
	dict = new dictionary(); // Initialize the dictionary
	int words_loaded = dict->LoadFile((char *) (DICTIONARY.c_str())); // Load the dictionary
	if (words_loaded <= 0) { // If the dictionary could not be loaded, we stop
		perror(std::string("An error occurred while opening the supplied dictionary '" + DICTIONARY + "'").c_str());
		exit(DICTIONARY_FAILURE);
	}
	std::cout << "Loaded " << words_loaded << " words!" << std::endl;
	// * End dictionary

	// * Start creating worker threads
	std::cout << "Creating " << MAX_WORKER_THREADS << " worker thread" << (MAX_WORKER_THREADS == 1 ? "" : "s") << "... ";
	pthread_t threads[MAX_WORKER_THREADS];

	for (pthread_t &thread: threads) {
		pthread_create(&thread, nullptr, WorkerThread, nullptr);
	}
	std::cout << "Completed!" << std::endl;
	// * End creating worker threads

	// * Start logger thread
	std::cout << "Creating logger thread... ";
	pthread_t logger_thread;
	pthread_create(&logger_thread, nullptr, LoggerThread, nullptr);
	std::cout << "Completed!" << std::endl;
	// * End logger thread

	// * Start creating TCP socket
	// Create a listening socket
	std::cout << "Creating TCP listening socket... ";
	int connection_socket = OpenListenerSocket(PORT, MAX_WORKER_THREADS);

	// Ensure the socket created is valid
	if (connection_socket == -1) {
		perror(std::string("Error occurred when creating server for port '" + std::to_string(PORT) + "'").c_str());
		exit(SOCK_CREATE_FAILURE);
	}
	std::cout << "Completed!" << std::endl;
	// * End creating TCP socket

	// Changing variables
	int client_socket;
	// If we hit this point, we've successfully created a listening TCP server.  Spinlock until the server is manually closed.
	std::cout << "Ready for client requests on port " << PORT << std::endl;
	while (true) {
		client_socket = accept(connection_socket, nullptr, nullptr);
		clock_t current_time = clock();

		if (client_socket < 0) {
			perror("An error occurred while opening a connection with a new connection.");
			continue;
		}

		pthread_mutex_lock(&worker_mutex);

		if (worker_queue->size() >= MAX_WORKER_QUEUE_SIZE) {
			std::cout << "Waiting for queue to client_removed before adding to the " << std::endl;
			pthread_cond_wait(&client_removed, &worker_mutex);
		}

		// Add the client socket to the worker's queue
		int priority = (PRIORITY_MODE ? Helpers::Generics::rng(1, 10) : 1);
		worker_entry new_entry(client_socket, priority, current_time);
		worker_queue->emplace(new_entry);
		std::cout << "Added socket " << client_socket << " to the queue!" << std::endl;

		// Signal to all threads that a client was added and unlock the worker_mutex
		pthread_cond_signal(&client_added);
		pthread_mutex_unlock(&worker_mutex);
	}
}