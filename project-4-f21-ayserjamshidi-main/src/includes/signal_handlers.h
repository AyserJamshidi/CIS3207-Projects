//
// Created by ayserjamshidi on 11/19/21.
//

#ifndef PROJECT_4_F21_AYSERJAMSHIDI_SIGNAL_HANDLERS_H
#define PROJECT_4_F21_AYSERJAMSHIDI_SIGNAL_HANDLERS_H

#include <sched.h>

class Handlers {
public:
	static void SignalGenerator(int main_proc, bool is_main);

	static void CreateSignalHandler(pid_t signal_generators[]);

	static void *ReceiveSignal(void *arg);

	static void SignalGenerator(int main_proc, const pid_t *generator_children);
};

#endif //PROJECT_4_F21_AYSERJAMSHIDI_SIGNAL_HANDLERS_H
