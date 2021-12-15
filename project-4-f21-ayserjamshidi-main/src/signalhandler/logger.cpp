//
// Created by ayserjamshidi on 11/18/21.
//

#include <chrono>
#include <thread>
#include <iostream>
#include <cstring>

#include "../includes/globals.h"
#include "../includes/logger.h"

int sigusr1_received = 0, sigusr2_received = 0;

long GetVectorAverage(std::vector<long> &vect) {
	if (vect.empty())
		return 0;

	long output = 0, initial_num = vect.at(0);

	for (long curNum: vect)
		output += curNum - initial_num;

	return output / (long) vect.size();
}

void Logger() {
//	int ind = 1;
	std::ofstream output_file("signalreceive_log.txt", std::ios_base::app);

	if (!output_file.is_open())
		perror("An error occurred while attempting to output to the log file 'signalreceive_log.txt'");

	std::vector<long> sigusr1_entries, sigusr2_entries;

	while (PROGRAM_RUNNING) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		pthread_mutex_lock(&LOGGER_LOCK);

		while (PROGRAM_RUNNING && LOGGER_QUEUE.size() < 16)
			pthread_cond_wait(&MAX_SIGNALS, &LOGGER_LOCK);

		if (!PROGRAM_RUNNING)
			break;

		while (!LOGGER_QUEUE.empty()) {
			// Acquire next in line log entry
			log_entry log = LOGGER_QUEUE.front();
			LOGGER_QUEUE.pop();

			// Output information to file and add current time to proper sigusr entry vector for averages later on
//			std::cout << ind++ << " ::: Thread [" << log.thread_id
//			          << "] received [" << log.signal
//			          << "] at runtime [" << log.time.count() << "ms]" << std::endl;

			output_file << " ::: Thread [" << log.thread_id
			            << "] received [" << log.signal
			            << "] at runtime [" << log.time.count() << "ms]" << std::endl;

			// Place the current time in its appropriate vector to average it later
			(strcmp(log.signal, "SIGUSR1") == 0 ? sigusr1_entries : sigusr2_entries).push_back(log.time.count());

		}

		// Output averages to file
//		std::cout << "Averages: [SIGUSR1] = [" << GetVectorAverage(sigusr1_entries)
//					<< "ms] || [SIGUSR2] = [" << GetVectorAverage(sigusr2_entries) << "ms]" << std::endl << std::endl;

		std::cout << "Total count: " << (SIGUSR1_COUNT + SIGUSR2_COUNT - 1) << std::endl;

		output_file << "Averages: [SIGUSR1] = [" << GetVectorAverage(sigusr1_entries)
		            << "ms] || [SIGUSR2] = [" << GetVectorAverage(sigusr2_entries) << "ms]" << std::endl << std::endl;

		// Increase received entries
		sigusr1_received += sigusr1_entries.size();
		sigusr2_received += sigusr2_entries.size();

		// Clean vectors for next run
		sigusr1_entries.clear();
		sigusr2_entries.clear();

		// Announce that we've emptied the signals so receivers can continue working
		pthread_cond_signal(&EMPTIED_SIGNALS);

		// Unlock the logger mutex
		pthread_mutex_unlock(&LOGGER_LOCK);
	}

	// Close the file so we can have successful output
	output_file.close();

	output_file.open("signalreceive_log.txt", std::ios_base::app);

	if (output_file.is_open()) {
		output_file << "[SIGUSR1] generated: " << SIGUSR1_COUNT << " || received: " << sigusr1_received << " || loss: " << (100 * (1 - ((double) sigusr1_received/(double)SIGUSR1_COUNT))) << std::endl;
		output_file << "[SIGUSR2] generated: " << SIGUSR2_COUNT << " || received: " << sigusr2_received << " || loss: " << (100 * (1 - ((double) sigusr2_received/(double)SIGUSR2_COUNT))) << std::endl;

		output_file.close();
	}


	// Announce departure
	std::cout << "Logger thread is peacefully shutting down..." << std::endl;
}
