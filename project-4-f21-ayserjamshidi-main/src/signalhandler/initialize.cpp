//
// Created by ayserjamshidi on 11/18/21.
//

#include <iostream>
#include <csignal>
#include <queue>
#include <thread>
#include <sys/wait.h>

#include "../includes/signal_handlers.h"
#include "../includes/errors.h"
#include "../includes/logger.h"
#include "../includes/log_entry.h"
#include "../includes/globals.h"

// Initialize global variables from globals.h
unsigned int    SIGUSR1_COUNT = 0,
				SIGUSR2_COUNT = 0;

pthread_mutex_t SIGUSR1_LOCK = PTHREAD_MUTEX_INITIALIZER,
				SIGUSR2_LOCK = PTHREAD_MUTEX_INITIALIZER,
				LOGGER_LOCK = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  MAX_SIGNALS = PTHREAD_COND_INITIALIZER,
				EMPTIED_SIGNALS = PTHREAD_COND_INITIALIZER;

std::queue<log_entry> LOGGER_QUEUE;

pthread_t receivers[4];

/**
 * Given a signal, the process will block it.
 *
 * @param sig Signal to block
 */
void BlockSignal(int sig) {
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, sig);
	sigprocmask(SIG_BLOCK, &sigset, nullptr);
}

void SignalHandlerTerminate(int i) {
//	std::cout << "Inside of Signal Handler TERM " << std::endl;
	PROGRAM_RUNNING = false;
	pthread_cond_signal(&EMPTIED_SIGNALS); // Release any receivers waiting for the logger to empty

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	pthread_cond_signal(&MAX_SIGNALS); // Release the logger in case it's waiting
}

void Handlers::CreateSignalHandler(pid_t signal_generators[]) {
	std::cout << "Signal handler initializing!" << std::endl;

	// Blocking SIGUSR1/2 before thread creation so we can unblock the desired SIGUSR later in their respective threads
	BlockSignal(SIGUSR1);
	BlockSignal(SIGUSR2);

	// Unblock SIGTERM
	sigset_t sigset;
	sigaddset(&sigset, SIGTERM);
	sigprocmask(SIG_UNBLOCK, &sigset, nullptr);

	// Ensures the child process captures SIGTERM and peacefully closes all threads before exiting.
	signal(SIGTERM, SignalHandlerTerminate);

	std::cout << "Creating 4 receiver threads..." << std::endl;

	for (int index = 0; index < 4; index++) {
		int sig = index < 3 ? SIGUSR1 : SIGUSR2;

		// Create receiver threads.  If something goes wrong, exit this process with an error message.
		if (pthread_create(&receivers[index], nullptr, Handlers::ReceiveSignal, &sig) != 0)
			Errors::ExitWithErrorMessage(Errors::PTHREAD_CREATE_FAILURE);
	}

	// Start logging
	Logger();

	for (int i = 0; i < 2; i++) {
		std::cout << "Waiting for the " << (i == 0 ? "first" : "second") << " child generator to be killed..." << std::endl;

		kill(signal_generators[i], SIGTERM);
		waitpid(signal_generators[i], nullptr, 0);
	}
}