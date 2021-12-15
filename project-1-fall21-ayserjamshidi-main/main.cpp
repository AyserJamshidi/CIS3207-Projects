//
// Created by Ace on 9/11/2021.
//
#include <bits/stdc++.h>

#include "config_file.h"
#include "model/job.h"  // Job implements job_event.h

// USER-CHANGABLE VARIABLES
const char *LOG_FILE = "log.txt";
const char *STAT_FILE = "stats.txt";

// Bootleg header
void
ComponentHandler(JobEvent event_type, Job &cur_job, std::queue<Job> &component_queue, bool &component_occupied, int component_min_time, int component_max_time, unsigned int &component_utilization,
                 std::queue<int> &component_queue_size_entry, unsigned int &component_size_max, int &component_job_count);

float GetQueueEntryAverage(std::queue<int> &component_queue);

void LogOutput(Job cur_job);

int randomNum(int minimum, int maximum);

// Config variables
int SEED, INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, QUIT_PROB, NETWORK_PROB, CPU_MIN, CPU_MAX, DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX, NETWORK_MIN, NETWORK_MAX;

// Simulation variables
int cur_job_num, sim_time;

// Queue variables
std::priority_queue<Job, std::vector<Job>, Job::Compare> sim_queue;
std::queue<Job> cpu_queue, disk1_queue, disk2_queue, network_queue;
bool cpu_occupied, disk1_occupied, disk2_occupied, network_occupied;

// Statistic variables
std::ofstream log_file;

//std::queue<int> cpu_queue_sizes, disk1_queue_sizes, disk2_queue_sizes, network_queue_sizes;
//float cpu_queue_avg_size, disk1_queue_avg_size, disk2_queue_avg_size, network_queue_avg_size;

int main() {
	config_file config;
	config.LoadFile();

	remove(LOG_FILE);
	log_file.open(LOG_FILE);

	// Initialize config variables
	SEED = config.GetValue("SEED");
	INIT_TIME = config.GetValue("INIT_TIME");
	FIN_TIME = config.GetValue("FIN_TIME");
	ARRIVE_MIN = config.GetValue("ARRIVE_MIN");
	ARRIVE_MAX = config.GetValue("ARRIVE_MAX");
	QUIT_PROB = config.GetValue("QUIT_PROB");
	NETWORK_PROB = config.GetValue("NETWORK_PROB");
	CPU_MIN = config.GetValue("CPU_MIN");
	CPU_MAX = config.GetValue("CPU_MAX");
	DISK1_MIN = config.GetValue("DISK1_MIN");
	DISK1_MAX = config.GetValue("DISK1_MAX");
	DISK2_MIN = config.GetValue("DISK2_MIN");
	DISK2_MAX = config.GetValue("DISK2_MAX");
	NETWORK_MIN = config.GetValue("NETWORK_MIN");
	NETWORK_MAX = config.GetValue("NETWORK_MAX");

	// Initialize simulation variables
	cur_job_num = 0;
	sim_time = INIT_TIME;

	// User error checking
	if (sim_time >= FIN_TIME) {
		std::cout << "Please set INIT_TIME to be less than FIN_TIME." << std::endl;
		return 0;
	}

	// Logging variables
	std::queue<int> cpu_queue_size_entry, disk1_queue_size_entry, disk2_queue_size_entry, network_queue_size_entry;
	unsigned int cpu_utilization, disk1_utilization, disk2_utilization, network_utilization;
	unsigned int cpu_size_max, disk1_size_max, disk2_size_max, network_size_max;
	int cpu_job_count, disk1_job_count, disk2_job_count, network_job_count;

	// Set up seed for pseudo-random values
	std::srand(SEED);

	if (config.isDebugging())
		std::cout << "Done initializing!  Starting simulation error checking." << std::endl;

	// * Create the first Job to start the simulation
	sim_queue.push(Job(++cur_job_num, sim_time));

	// The simulation loop
	while (!sim_queue.empty() && sim_time < FIN_TIME) {
		// Get the oldest Job then pop it out of queue
		Job cur_job = sim_queue.top();
		sim_queue.pop();

		switch (cur_job.GetEvent()) {
			case JobEvent::JOB_ARRIVAL: {
				// Create a new job with an incremented expected simulation time
				LogOutput(cur_job);
				sim_time = cur_job.GetExecutionTime();
				sim_queue.push(Job(++cur_job_num, sim_time + randomNum(ARRIVE_MIN, ARRIVE_MAX)));

				// Reuse the same event (to preserve the time) and set its event to CPU_ARRIVAL
				cur_job.SetEvent(JobEvent::CPU_ARRIVAL);
				sim_queue.push(cur_job);
				break;
			}
			case JobEvent::JOB_QUIT: {
				// This job's memoryspace will be freed as it will be completely out of scope
				LogOutput(cur_job);
				break;
			}
			case JobEvent::CPU_ARRIVAL: {
				// Logging
				cpu_queue_size_entry.push(cpu_queue.size());

				if (cpu_queue.size() > cpu_size_max)
					cpu_size_max = cpu_queue.size();

				if (cpu_occupied) {  // If the CPU component_handlers is taken
					cpu_queue.push(cur_job);
				} else {
					int execution_time = randomNum(CPU_MIN, CPU_MAX);
//					cpu_utilization += execution_time;

					LogOutput(cur_job);
					cur_job.SetEvent(CPU_FINISH);
					cur_job.SetExecutionTime(sim_time + execution_time);
					sim_queue.push(cur_job);
					cpu_occupied = true;  // Set CPU component_handlers to busy
				}
				break;
			}
			case JobEvent::CPU_FINISH: {
				if (cpu_queue.size() > cpu_size_max)
					cpu_size_max = cpu_queue.size();

				int prob_outcome = randomNum(0, 100);
				LogOutput(cur_job);

				if (prob_outcome <= QUIT_PROB) {  // Quit the simulation
					cur_job.SetEvent(JobEvent::JOB_QUIT);
				} else if (prob_outcome <= NETWORK_PROB) {  // Go to network component_handlers
					cur_job.SetEvent(JobEvent::NETWORK_ARRIVAL);
				} else {  // Go to disk component_handlers
					JobEvent desired_event;

					/*
					 * If disk 1 and 2 are the same size, is_disk1 = true if randomNum returns 0, false otherwise.
					 * If disk 1 and 2 are not the same, is_disk1 = true if disk1_component.size < disk2.size, false otherwise.
					 */
					bool is_disk1 = (disk1_queue.size() == disk2_queue.size()) ? randomNum(0, 1) == 0 : disk1_queue.size() < disk2_queue.size();
					desired_event = (is_disk1 ? JobEvent::DISK1_ARRIVAL : JobEvent::DISK2_ARRIVAL);

					cur_job.SetEvent(desired_event);
				}
				cpu_job_count++;
				sim_queue.push(cur_job);

				if (!cpu_queue.empty()) {
					// Logging
					cpu_queue_size_entry.push(cpu_queue.size());

					Job next_job = cpu_queue.front();
					cpu_queue.pop();
					LogOutput(next_job);

					int execution_time = randomNum(CPU_MIN, CPU_MAX);
					sim_time = cur_job.GetExecutionTime();
					next_job.SetExecutionTime(sim_time + execution_time);
					cpu_utilization += execution_time;
					next_job.SetEvent(JobEvent::CPU_FINISH);
					sim_queue.push(next_job);
				} else {
					cpu_occupied = false;  // Free the CPU!
				}

				break;
			}
			case JobEvent::DISK1_ARRIVAL:
			case JobEvent::DISK1_FINISH:
				ComponentHandler(JobEvent::DISK1_FINISH, cur_job, disk1_queue, disk1_occupied, DISK1_MIN, DISK1_MAX, disk1_utilization, disk1_queue_size_entry, disk1_size_max, disk1_job_count);
				break;
			case JobEvent::DISK2_ARRIVAL:
			case JobEvent::DISK2_FINISH:
				ComponentHandler(JobEvent::DISK2_FINISH, cur_job, disk2_queue, disk2_occupied, DISK2_MIN, DISK2_MAX, disk2_utilization, disk2_queue_size_entry, disk2_size_max, disk2_job_count);
				break;
			case JobEvent::NETWORK_ARRIVAL:
			case JobEvent::NETWORK_FINISH:
				ComponentHandler(JobEvent::NETWORK_FINISH, cur_job, network_queue, network_occupied, NETWORK_MIN, NETWORK_MAX, network_utilization, network_queue_size_entry, network_size_max,
				                 network_job_count);
				break;

			case JobEvent::QUIT:
				break;
		}
	}

	// Output log
	if (!log_file.is_open())
		std::cout << LOG_FILE << " wasn't able to be made!" << std::endl;
	else
		log_file.close();


	// Output statistics
	remove(STAT_FILE);
	std::ofstream stats;
	stats.open(STAT_FILE);

	if (!stats.is_open()) {
		std::cout << STAT_FILE << " wasn't able to be made!";
	} else {
		stats << "Average & Maximum Size Per Queue (Average / Max)" << std::endl;

		stats << "CPU: (" << GetQueueEntryAverage(cpu_queue_size_entry) << " / " << cpu_size_max << ")" << std::endl;
		stats << "DISK1: (" << GetQueueEntryAverage(disk1_queue_size_entry) << " / " << disk1_size_max << ")" << std::endl;
		stats << "DISK2: (" << GetQueueEntryAverage(disk2_queue_size_entry) << " / " << disk2_size_max << ")" << std::endl;
		stats << "NETWORK: (" << GetQueueEntryAverage(network_queue_size_entry) << " / " << network_size_max << ")" << std::endl;

		stats << std::endl;

		// Utilization
		float runtime = FIN_TIME - INIT_TIME;
		stats << "Utilization" << std::endl;
		stats << "CPU: " << ((float) cpu_utilization / runtime) << std::endl;
		stats << "DISK1: " << ((float) disk1_utilization / runtime) << std::endl;
		stats << "DISK2: " << ((float) disk2_utilization / runtime) << std::endl;
		stats << "NETWORK: " << ((float) network_utilization / runtime) << std::endl;

		stats << std::endl;

		// Throughput
		float per_unit_time = 10;
		stats << "Throughput per " << (FIN_TIME / per_unit_time) << " units of time" << std::endl;
		stats << "CPU: " << (float) cpu_job_count / per_unit_time << std::endl;
		stats << "DISK1: " << (float) disk1_job_count / per_unit_time << std::endl;
		stats << "DISK2: " << (float) disk2_job_count / per_unit_time << std::endl;
		stats << "NETWORK: " << (float) network_job_count / per_unit_time << std::endl;
	}

	return 0;  // Success
}

void
ComponentHandler(JobEvent event_type, Job &cur_job, std::queue<Job> &component_queue, bool &component_occupied, int component_min_time, int component_max_time, unsigned int &component_utilization,
                 std::queue<int> &component_queue_size_entry, unsigned int &component_size_max, int &component_job_count) {
	int execution_time;

	// Logging
	component_queue_size_entry.push(component_queue.size());

	if (component_queue.size() > component_size_max)
		component_size_max = component_queue.size();

	if (ToString(cur_job.GetEvent()).find("arrive") != std::string::npos) { // Arrived
		if (component_occupied) {  // If the CPU component_handlers is taken
			component_queue.push(cur_job);
		} else {
			// Logging
			execution_time = randomNum(component_min_time, component_max_time);
			component_utilization += execution_time;

			// Print to log.txt and stdout
			LogOutput(cur_job);

			// Change the event so we can reuse this job object
			cur_job.SetEvent(event_type);

			// Add the execution time to the simulation time to progress the simulation
			sim_time = cur_job.GetExecutionTime();
//			sim_time += execution_time;

			// Set the job's expected execution time to sim_time since a job's execution time should be CUR_TIME + EXPECTED_EXEC_TIME
			cur_job.SetExecutionTime(sim_time + execution_time);

			// Set the component to component_occupied
			component_occupied = true;

			// Add the job to the priority component_queue.
			sim_queue.push(cur_job);
		}
	} else { // Finished
		// Logging
		component_job_count++;

		// Log the job then add a CPU arrival job to the simulation component_queue
		LogOutput(cur_job);
		cur_job.SetEvent(JobEvent::CPU_ARRIVAL);
		sim_queue.push(cur_job);

		if (!component_queue.empty()) {
			Job next_job = component_queue.front();
			component_queue.pop();
			LogOutput(cur_job);

			execution_time = randomNum(component_min_time, component_max_time);
			sim_time = cur_job.GetExecutionTime();
			next_job.SetExecutionTime(sim_time + execution_time);
			component_utilization += execution_time;
			next_job.SetEvent(event_type);
			sim_queue.push(next_job);
		} else {
			component_occupied = false;
		}
	}
}

void LogOutput(Job cur_job) {
	std::string message = "At time \t" + std::to_string(sim_time) + (sim_time < 100 ? " " : "") + (sim_time < 10 ? " " : "")
	                      + " \t\tJob" + std::to_string(cur_job.GetNumber()) + "\t\t" + ToString(cur_job.GetEvent()) + "\n";

	std::cout << message;

	if (log_file.is_open())
		log_file << message;
}

float GetQueueEntryAverage(std::queue<int> &component_queue) {
	int queue_total = 0, queue_entry_amount = 0;

	while (!component_queue.empty()) {
		queue_total += component_queue.front();
		component_queue.pop();
		queue_entry_amount++;
	}

	if (queue_entry_amount == 0)
		return 0;

	return (float) queue_total / (float) queue_entry_amount;
}

// Taken from DiscreteEventSimulation.pdf sample code
int randomNum(int minimum, int maximum) {
	return minimum + std::rand() % (maximum - minimum + 1);
}