//
// Created by ace on 12/5/21.
//

#ifndef PROJECT_4_F21_AYSERJAMSHIDI_LOG_ENTRY_H
#define PROJECT_4_F21_AYSERJAMSHIDI_LOG_ENTRY_H

#include <chrono>

struct log_entry {
	std::chrono::duration<double, std::milli> time;
	unsigned long thread_id;
	const char* signal;
};

#endif //PROJECT_4_F21_AYSERJAMSHIDI_LOG_ENTRY_H
