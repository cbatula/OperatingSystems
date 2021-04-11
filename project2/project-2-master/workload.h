#ifndef WORKLOAD
#define WORKLOAD

#include "process.h"
#include <stdio.h>

typedef struct {
	char process_name;
	float arrival_time;
	float service_time;
	int priority;
	int start_queue; // queue that this process was started from (HPF aging)
	int start_time;
	int end_time;
  	float turnaround_time;
	float response_time;
	float wait_time;
	int enqueue_time;
} process_struct;

process_struct* create_workload(int size);

void free_workload(process_struct* workload, int size);

void print_workload(process_struct* workload, int size);
#endif
