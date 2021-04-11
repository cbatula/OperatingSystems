#include <iostream>
#include <array>
#include <list>
#include <vector>
#include <memory>
#include <map>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

#include <assert.h>

#include "Job.h"
#include "ReplacementAlgorithm.h"
#include "Simulator.h"

#define JOB_COUNT 150

float hit_ratio_total = 0;
float miss_ratio_total = 0;
int started_job = 0;


void runAlgo(const std::list<Job> &jobs, ReplacementAlgoPtr algo)
{
	// Create free page list with 100 pages, 1MB each
	Simulator sim(100, jobs, algo);

	for (size_t i = 0; i < 10 * 60; i++)
	{
		sim.tick();
	}

	printf("\t\tmemory hit ratio: %f\n", sim.hit_ratio());
	hit_ratio_total += sim.hit_ratio();
	printf("\t\tmemory miss ratio: %f\n", sim.miss_ratio());
	miss_ratio_total += sim.miss_ratio(); 
	printf("\t\ttotal started jobs: %i\n", sim.totalStarted());
	started_job += sim.totalStarted();
	// TODO: print results?
	// results are printed on the fly
}

const size_t RUN_COUNT = 5;

// int main(int argc, char *argv[]) // Do we need arguments
int main(void)
{
	std::srand(std::time(nullptr));

	for (size_t i = 0; i < RUN_COUNT; i++)
	{
		// Replacement algos to run
		std::vector<ReplacementAlgoPtr> algos = {
			ReplacementAlgoPtr(new RandomReplacement()),
			ReplacementAlgoPtr(new LFUReplacement()),
			ReplacementAlgoPtr(new MFUReplacement()),
			ReplacementAlgoPtr(new LRUReplacement()),
			ReplacementAlgoPtr(new FIFOReplacement())
		};

		// Generate workload
		auto jobs = Job::generateJobs(JOB_COUNT);

		printf("\t%i.\n", i+1);
		for (auto &&algo : algos)
		{
			std::cout << "\n===========================================\n";
			std::cout << "******Starting " <<algo->name() <<" ******\n";
			std::cout << "\n===========================================\n\n";
			runAlgo(jobs, algo);
		}
	}
	//printf("\tavg:\n");
	//printf("\t\tavg mem hit ratio: %f\n", hit_ratio_total / RUN_COUNT);
	//printf("\t\tavg mem miss ratio: %f\n", miss_ratio_total / RUN_COUNT);
	//printf("\t\tavg started jobs: %f\n", (float)started_job / RUN_COUNT);
}
