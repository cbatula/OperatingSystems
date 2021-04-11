#include "Process.h"

#include <cstdlib>
#include <iostream>

Process::Process(Job job) : job(job), page_in_mem(job.proc_size),
							remaining_time_ms(job.service_duration_ms)
{
	// last_access = std::rand() % job.proc_size;
	last_access = 0; // processes always access page 0 first

	// start all pages not allocated
	for (size_t i = 0; i < page_in_mem.size(); i++)
	{
		page_in_mem[i] = false;
	}
}

bool Process::isCompleted() const
{
	return remaining_time_ms <= 0;
}

size_t Process::residentSize() const
{
	size_t i = 0;
	for (auto &&pg : page_in_mem)
	{
		if (pg)
		{
			i++;
		}
	}
	return i;
}

size_t Process::getRandomAccess()
{
	int delta;
	if (rand() % 10 <= 7)
	{
		delta = rand() % 3 - 1; // -1 to 1
	}
	else
	{
		delta = rand() % (job.proc_size - 2) + 2; // 2 to job.proc_size
	}

	last_access = (last_access + delta) % job.proc_size;

	return last_access;
}

bool Process::isSwappedIn(size_t page_id) const
{
	return page_in_mem[page_id];
}

void Process::swapIn(size_t page_id)
{
	page_in_mem[page_id] = true;
}

void Process::swapOut(size_t page_id)
{
	page_in_mem[page_id] = false;
}
