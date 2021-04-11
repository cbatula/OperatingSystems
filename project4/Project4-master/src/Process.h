#pragma once

#include <list>
#include <vector>
#include <memory>

#include "Job.h"

// A process is the execution of a job
class Process
{
	size_t last_access;
public:
	const Job job;
	std::vector<bool> page_in_mem;
	float remaining_time_ms;

	size_t start_time; 


	Process(Job job);

	bool isCompleted() const;
	bool isSwappedIn(size_t page_id) const;
	size_t residentSize() const;

	size_t getRandomAccess();
	void swapIn(size_t page_id);
	void swapOut(size_t page_id);

};

typedef std::shared_ptr<Process> ProcPtr;
