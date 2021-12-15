//
// Created by ayserjamshidi on 11/18/21.
//
#include <iostream> // I/O
#include <unistd.h> // Fork, exec, sleep, ...
#include <cstring>
#include <sys/wait.h>
#include <thread>

#include "includes/errors.h"
#include "includes/signal_handlers.h"
#include "includes/globals.h"
#include "includes/colors.h"

bool PROGRAM_RUNNING = true;
bool TIME_TRIAL = false;

std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> START_TIME = std::chrono::high_resolution_clock::now();

pid_t signal_generators[3];

/**
 * @brief Toggles this child's copy of PROGRAM_RUNNING to peacefully exit.
 */
void TerminateReceived(int i) {
	if (i == SIGTERM){
		PROGRAM_RUNNING = false;
	}
	else{
		printf("NOT CORRECT SIGNAL\n");
	}
}

void CreateSignalGenerators(pid_t signal_handler) {
	// creation of signal generator processes
	for (int i = 0; i < 2; i++) {
		signal_generators[i] = fork();

		if (signal_generators[i] == 0) {
//			signal(SIGTERM, TerminateReceived);
			Handlers::SignalGenerator(signal_handler, false);
			_Exit(0); // We reserve exit() for the parent process.  Children use _Exit()
		} else if (signal_generators[i] < 0) {
			std::cout << "Fork failed." << std::endl;
		}
	}
}

int main(int argc, char **argv) {
	// Remove the log file so we can have a nice, clean output
	remove("signalreceive_log.txt");

	// If user put 0 as first parameter, time trial mode, otherwise, 100k simulation
	if (argc > 1 && strcmp(argv[1], "0") == 0) {
		TIME_TRIAL = true;
		std::cout << BLUE << "Executing for 30 seconds." << RESET << std::endl;
	} else {
		std::cout << BLUE << "Executing until 100,000 signals." << RESET << std::endl;
	}

	signal(SIGTERM, TerminateReceived);

	// Assign the third generator the main process PID
	signal_generators[2] = getpid();

	// Create a child
	pid_t signal_handler = fork();

	if (signal_handler == 0) { // This process is a child!
		/*
		 * Create 4 threads within this child (5 in total).
		 * 2 receiving SIGUSR1
		 * 2 receiving SIGUSR2
		 *
		 * The child's thread will handle logging
		 */
		Handlers::CreateSignalHandler(signal_generators);

		// Exit the child peacefully
		_Exit(-2);
	} else if (signal_handler < 0) {
		Errors::ExitWithErrorMessage(Errors::FORK_FAILURE);
	}

	// Wait for the child to be created
	sleep(1);

	CreateSignalGenerators(signal_handler);

	// We are finished initializing.  Act as a signal generating process now.
	Handlers::SignalGenerator(signal_handler, signal_generators);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	// If we've hit this point, we're exiting the program peacefully.
	// Kill child that handled logging + 4 receivers
	std::cout << "Attempting to close the primary logger and receiver handler..." << std::endl;
	kill(signal_handler, SIGTERM);
	waitpid(signal_handler, nullptr, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	std::cout << GREEN << "All children and their threads have been closed peacefully!  Exiting..." << RESET << std::endl;
}
