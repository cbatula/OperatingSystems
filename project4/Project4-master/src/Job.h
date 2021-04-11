#pragma once
#include <string>
#include <list>
#include <vector>

// A job is the definition of a piece of work
class Job
{
public:
	const std::string name;
	const size_t proc_size; // pages that could be accessed by process
	const float arrival_time_ms; // in ms
	const float service_duration_ms; // in ms

	Job(std::string name, size_t proc_size, float arrival_time_ms, 
		float service_duration_ms);

	static std::list<Job> generateJobs(int num_jobs);
};

std::vector<Job> sortJobVector(std::vector<Job>);
