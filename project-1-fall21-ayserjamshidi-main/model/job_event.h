//
// Created by aceja on 9/17/2021.
//
#include <string>

enum JobEvent {
	JOB_ARRIVAL,
	JOB_QUIT,
	CPU_ARRIVAL,
	CPU_FINISH,
	DISK1_ARRIVAL,
	DISK1_FINISH,
	DISK2_ARRIVAL,
	DISK2_FINISH,
	NETWORK_ARRIVAL,
	NETWORK_FINISH,
	QUIT
};

inline std::string ToString(JobEvent job_event) {
	switch (job_event) {
		case JOB_ARRIVAL:
			return "arrived in simulation";
		case JOB_QUIT:
			return "quit the simulation";
		case CPU_ARRIVAL:
			return "arrived in CPU";
		case CPU_FINISH:
			return "finished in CPU";
		case DISK1_ARRIVAL:
			return "arrived in Disk1";
		case DISK1_FINISH:
			return "finished in Disk1";
		case DISK2_ARRIVAL:
			return "arrived in Disk2";
		case DISK2_FINISH:
			return "finished in Disk2";
		case NETWORK_ARRIVAL:
			return "arrived in Network";
		case NETWORK_FINISH:
			return "finished in Network";
		case QUIT:
			return "simulation finished";
		default:
			return "UNKNOWN";
	}
}