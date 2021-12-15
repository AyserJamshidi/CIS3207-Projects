//
// Created by aceja on 9/17/2021.
//

#include "job.h"

int Job::GetNumber() const {
	return job_num;
}

JobEvent Job::GetEvent() {
	return cur_event;
}

void Job::SetEvent(JobEvent given_event) {
	cur_event = given_event;
}

int Job::GetExecutionTime() const {
	return execution_time;
}

void Job::SetExecutionTime(int execution_time) {
	this->execution_time = execution_time;
}
