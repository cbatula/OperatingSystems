#include "Job.h"

Job::Job(std::string name, size_t proc_size, float arrival_time_ms,
		 float service_duration_ms)
	: name(name), proc_size(proc_size), arrival_time_ms(arrival_time_ms),
	  service_duration_ms(service_duration_ms)
{
}

const static size_t JOB_COUNT = 150;
const static size_t PROC_SIZES[] = {5, 11, 17, 31};	  // in pages
const static size_t SERVICE_DURS[] = {1, 2, 3, 4, 5}; // in seconds
const static size_t MAX_ARRIVAL_TIME_MS = 3000; // 60s in ms

// generates and returns a list<Job> 
// sorted by lowest arrival time first
std::list<Job> Job::generateJobs(int num_jobs)
{
	std::vector<Job> job_vector;
	for (int i = 0; i < num_jobs; i++){ // generates jobs and adds them to a vector
		size_t proc_size = PROC_SIZES[rand() % sizeof(PROC_SIZES)/sizeof(PROC_SIZES[0])]; // selects random proc_size
		float arrival_time = (rand() / (float)RAND_MAX) * MAX_ARRIVAL_TIME_MS; // selects random arrival_time
		float service_duration = SERVICE_DURS[rand() % sizeof(SERVICE_DURS)/sizeof(SERVICE_DURS[0])]* 1000; // selects random duration


		Job new_job(std::to_string(i), proc_size, arrival_time, service_duration);
		job_vector.push_back(new_job);
	}
	std::vector<Job> sorted_jobs = sortJobVector(job_vector); // sorts generated jobs

	std::list<Job> job_list(sorted_jobs.begin(), sorted_jobs.end()); // convert from vector to list

	return job_list;
}


// merge sorts a vector<> of Jobs by lowest arrival time first
std::vector<Job> sortJobVector(std::vector<Job> job_vector) {
	if (job_vector.size() <= 1) // terminate condition
		return job_vector;

	// sort each half 
	int half = job_vector.size()/2;
	std::vector<Job> a(job_vector.begin(), job_vector.begin() + half);
	std::vector<Job> b(job_vector.begin() + half, job_vector.end());
	std::vector<Job> c = sortJobVector(a); 
	std::vector<Job> d = sortJobVector(b);

	std::vector<Job> e;
	size_t i, j;
	for (i = 0, j = 0; i < c.size() && j < d.size();){ // merge stage
		if (c[i].arrival_time_ms < d[j].arrival_time_ms) {
			e.push_back(c[i]);
			i++;
		}
		else {
			e.push_back(d[j]);
			j++;
		}
	}
	if (i < c.size()) { // c hasn't been fully added to e yet
		for (; i < c.size(); i++)
			e.push_back(c[i]);
	}
	else { // d hasn't been fully added to e yet
		for (; j < d.size(); j++) 
			e.push_back(d[j]);
	}
	return e;
}
