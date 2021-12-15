//
// Created by Ace on 11/25/2021.
//

#ifndef PROJECT_4_F21_AYSERJAMSHIDI_GLOBALS_H
#define PROJECT_4_F21_AYSERJAMSHIDI_GLOBALS_H

#include <queue>
#include <chrono>
#include <csignal>
#include <fstream>
#include "../includes/log_entry.h"

extern bool PROGRAM_RUNNING, TIME_TRIAL;

// Start time
extern std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> START_TIME;

// SIGUSR counters
extern unsigned int SIGUSR1_COUNT,
					SIGUSR2_COUNT;

// Mutex locks
extern pthread_mutex_t  SIGUSR1_LOCK,
						SIGUSR2_LOCK,
						LOGGER_LOCK;

// Thread signals
extern pthread_cond_t   MAX_SIGNALS,
						EMPTIED_SIGNALS;


// Logger queue
extern std::queue<log_entry> LOGGER_QUEUE;

#endif //PROJECT_4_F21_AYSERJAMSHIDI_GLOBALS_H
