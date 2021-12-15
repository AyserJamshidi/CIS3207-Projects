//
// Created by ayserjamshidi on 11/18/21.
//

#include <iostream>
#include <csignal>
#include <chrono>
#include <bits/stdc++.h>

#include "../includes/signal_handlers.h" // Signal handlers
#include "../includes/globals.h"
#include "../includes/helpers.h"

struct signal_parameters {
	int sig;
};

/**
 * Unblocks/Unmasks the given signal for this thread
 *
 * @param sig Signal to unblock
 */
void UnblockSignal(int sig) {
	sigset_t sigset;
	sigaddset(&sigset, sig);
	sigprocmask(SIG_UNBLOCK, &sigset, nullptr);
}

void HandleSignal(int sig) {

	// Handle stopping the program via COUNT
	if (!TIME_TRIAL && (SIGUSR1_COUNT + SIGUSR2_COUNT == 100000)) {
		PROGRAM_RUNNING = false; // Ensure all threads stop spin-locking
		return;
//		pthread_cond_signal(&MAX_SIGNALS); // Let the logger run, it'll close itself
//		return;
	}

	bool is_sig1 = (sig == SIGUSR1);

	// Pointers to the current sigusr# sigusr_counter/mutex we want
	unsigned int *sigusr_counter = (is_sig1 ? &SIGUSR1_COUNT : &SIGUSR2_COUNT);
	pthread_mutex_t *sigusr_lock = (is_sig1 ? &SIGUSR1_LOCK : &SIGUSR2_LOCK);

	// Lock, add to the desired SIGUSR sigusr_counter and then unlock its respective mutex
	pthread_mutex_lock(sigusr_lock);
	(*sigusr_counter)++;
	pthread_mutex_unlock(sigusr_lock);

	pthread_mutex_lock(&LOGGER_LOCK);
	while (LOGGER_QUEUE.size() == 16) {
		pthread_cond_signal(&MAX_SIGNALS);
		pthread_cond_wait(&EMPTIED_SIGNALS, &LOGGER_LOCK); // wait for the logger thread to tell us that the queue is empty
	}

	log_entry new_log = {
			std::chrono::high_resolution_clock::now().time_since_epoch() - START_TIME.time_since_epoch(),
			pthread_self(),
			Helpers::Generics::SigusrText(sig)
	};

	LOGGER_QUEUE.push(new_log);
	pthread_mutex_unlock(&LOGGER_LOCK);
}

void *Handlers::ReceiveSignal(void *arg) {
	int sig = ((struct signal_parameters *) arg)->sig;

	// If our sig is SIGUSR1, block SIGUSR2, otherwise block SIGUSR1 (Signal masking)
	UnblockSignal((sig == SIGUSR1) ? SIGUSR1 : SIGUSR2);

	std::this_thread::sleep_for(std::chrono::milliseconds(Helpers::Generics::rng(10, 300)));
	// Set up signal handling to a function
	std::cout << "Receiver thread [" << pthread_self() << "] for [" << Helpers::Generics::SigusrText(sig) << "] initialized and waiting for signals!" << std::endl;
	signal(sig, HandleSignal);

	// We will break out of this loop when the main thread/child receives SIGTERM.
	while (PROGRAM_RUNNING)
		Helpers::Generics::sleep();

	std::cout << "Receiver thread [" << pthread_self() << "] is peacefully shutting down..." << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	pthread_cond_signal(&MAX_SIGNALS); // Release the logger in case it's waiting

	return nullptr;
}