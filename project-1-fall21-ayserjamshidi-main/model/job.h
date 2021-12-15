//
// Created by aceja on 9/17/2021.
//
#include <iostream>

#include "job_event.h"

class Job {
private:
	int job_num;
	JobEvent cur_event;
	int execution_time;

public:
	struct Compare {
		bool operator()(Job job1, Job job2) {
			return job1.GetExecutionTime() > job2.GetExecutionTime();
		}
	};

	Job(int job_num, int execution_time) {
		this->job_num = job_num;
		this->cur_event = JobEvent::JOB_ARRIVAL;
		this->execution_time = execution_time;
	}

	int GetNumber() const;
	JobEvent GetEvent();
	void SetEvent(JobEvent given_event);
	int GetExecutionTime() const;
	void SetExecutionTime(int execution_time);
};