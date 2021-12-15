//
// Created by ayserjamshidi on 11/19/21.
//

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "../includes/signal_handlers.h"
#include "../includes/helpers.h"
#include "../includes/globals.h"

int children[2];

void Handlers::SignalGenerator(int main_proc, const pid_t generator_children[]) {
	children[0] = generator_children[0];
	children[1] = generator_children[1];

	SignalGenerator(main_proc, true);
}

void GeneratorTerminate(int i) {
	PROGRAM_RUNNING = false;
}

void Handlers::SignalGenerator(int main_proc, bool is_main) {
	// If we're the parent, do nothing, otherwise set up a SIGTERM signal handler
	if (is_main)
		std::cout << "Parental ";
	else {
		std::cout << "Child ";
		signal(SIGTERM, GeneratorTerminate);
	}

	std::cout << "SignalGenerator started" << std::endl;

	while (PROGRAM_RUNNING) {
		Helpers::Generics::sleep();
		if (is_main) {
			std::chrono::duration<double, std::milli> time = std::chrono::high_resolution_clock::now().time_since_epoch() - START_TIME.time_since_epoch();

			// Handle stopping the program via TIME (30seconds)
			if (TIME_TRIAL && time.count() > 30000)
				PROGRAM_RUNNING = false; // Ensure all threads stop spin-locking
		}

		if (!PROGRAM_RUNNING)
			break;

		int sig = Helpers::Generics::rng(0, 1) ? SIGUSR1 : SIGUSR2;

		// Send a signal to the main process
		kill(main_proc, sig);
	}

	if (is_main) {
		std::cout << "Waiting for first child generator to be killed..." << std::endl;
		kill(children[0], SIGTERM);
		waitpid(children[0], nullptr, 0);

		std::cout << "Waiting for second child generator to be killed..." << std::endl;
		kill(children[1], SIGTERM);
		waitpid(children[1], nullptr, 0);
	} else
		std::cout << "Child SignalGenerator PID " << getpid() << " is peacefully shutting down..." << std::endl;

	Helpers::Generics::sleep();
}