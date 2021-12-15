//
// Created by lmfaoown on 11/12/21.
//

#ifndef PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_SERVER_STRUCTURES_H
#define PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_SERVER_STRUCTURES_H

#include <ctime>
#include <string>

struct logger_entry {
	std::string word;
	std::string result;
	clock_t arrival_time, completed_time;
};

struct worker_entry {
	// Client socket
	int socket;

	// Client priority
	int priority;

	// The instructions never tell us to use this, just to store it...
	std::time_t start_time;

	worker_entry(int socket, int priority, clock_t time) {
		this->socket = socket;
		this->priority = priority;
		this->start_time = time;
	}
};

struct compare {
	// Comparator for priority queue.  Moves the highest priority towards the top of the queue.
	bool operator()(worker_entry job1, worker_entry job2) {
		return job1.priority < job2.priority;
	}
};

#endif //PROJECT_3_NETWORKED_SPELL_CHECKER_AYSERJAMSHIDI_SERVER_STRUCTURES_H
