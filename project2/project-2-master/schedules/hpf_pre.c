#include "../process.h"
#include "../workload.h"
#include "hpf_pre.h"
#include "hpf_nonpre.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


float total_throughput = 0;
float total_turnaround = 0;
float total_wait = 0;
float total_response = 0;

void run_hpf_pre() {
	for(int i = 0; i < 5; i++) {
		printf("\n\n\n============HPF_PRE run %d================\n\n", i+1);
  		hpf_pre();
	}
	printf("\n=========================================\n\n");
	printf("~~~~~~~~~~Averages for all 5 runs~~~~~~~~~~\n\n");
	printf("Avg Turnaround time = %f\n", total_turnaround/5);
	printf("Avg Wait Time = %f\n", total_wait/5);
	printf("Avg Response Time = %f\n", total_response/5);
	printf("Avg Throughput = %f\n", total_throughput/5);
}

void hpf_pre(){
	char process_record[PROCESS_RECORD_LENGTH];
	int complete[TOTAL_PROCESSES] = {0};

	// ******* SETUP *********
	// initialize process record
	// 255 is reserved for no processes
	for (int i = 0; i < PROCESS_RECORD_LENGTH; i++) {
		process_record[i] = 255;
	}

	// generate sorted array of process_struct s
	// TRY NOT TO GENERATE MORE THAN 255 process_structs(char limitations)
	process_struct* processes = create_workload(TOTAL_PROCESSES);
	// change the process names to numbers
	// creates a queue of processes by arrival time
	queue* process_queue = init_queue();
	for (int i = 0; i < TOTAL_PROCESSES; i++) {
		processes[i].process_name = i;
		enqueue(process_queue, &processes[i]);
	}
	// create four queues; 1 for each priority
	queue* prio_queues[NUM_PRIO_LEVELS];
	for (int i = 0; i < NUM_PRIO_LEVELS; i++)
		prio_queues[i] = init_queue(); // initializes each queue
	// create queue for completed tasks
	queue* completed_processes = init_queue();

	// ****** START ONE LOOP ITERATION / QUANTUM ******
	process_struct *p, *head_process, *active_process = NULL;
	int max_quanta = 0;
	// keeps track of time left for each process
	float time_left[255];
	process_struct *ps;	
	for(int i = 0; i < TOTAL_PROCESSES; i++) {
		ps = &processes[i];
		time_left[i] = ps->service_time;
	}

	// keeps running while within time limit/a process is still running
	for (int i = 0; i < QUANTA_LIMIT || active_process != NULL; i++) {
		max_quanta = i + 1;  // saves total quanta run for
		
		// checks the active_process
		if (active_process != NULL) {
			// update process record at each quanta
			process_record[i-1] = active_process->process_name;
			// checks is current active_process has ended
			if (time_left[active_process->process_name] <= 1) {
				time_left[active_process->process_name] = 0;
				// active_process has ended so added
				// to completed_processes then marked as completed
				active_process->end_time = i;
				active_process->turnaround_time = active_process->end_time -
								active_process->arrival_time;
				active_process->wait_time = active_process->turnaround_time - 
								active_process->service_time;
				active_process->response_time = active_process->start_time - 
									active_process->arrival_time;
				if (complete[active_process->process_name] == 0) {
					enqueue(completed_processes, active_process);
					complete[active_process->process_name]++;
				}
				active_process = NULL;
			}
			else {
				time_left[active_process->process_name]-=1.0;
				//removes from head of queue
				enqueue(prio_queues[active_process->priority-1], active_process); 
			}
		}

		// ***AGING ADDED***	
		// updates priority queues at each quanta
		// while process_queue is not empty and first process has already arrived
		float new_arrival_time;
		while((i < QUANTA_LIMIT) && !is_empty(process_queue) && (process_queue->head->process->arrival_time < i)) {
			p = dequeue(process_queue); // dequeues from process queue
			new_arrival_time = p->arrival_time;
			for(int j = 1; j < NUM_PRIO_LEVELS; j++) {
				while(!is_empty(prio_queues[j]) && prio_queues[j]->head->process->enqueue_time
										+ AGE_OUT_TIME < new_arrival_time) {
					head_process = dequeue(prio_queues[j]);
					enqueue(prio_queues[j-1], head_process);
					head_process->enqueue_time += AGE_OUT_TIME;
				}
			}
			enqueue(prio_queues[p->priority-1], p); // enqueues into correct prio queue
			p->enqueue_time = p->arrival_time;
		}
		
		// check if need to dequeue/start new task
		if (active_process == NULL) {  
			// scans each priority queues until finds first available process
			// sets the process as the active process
			for (int j = 0; j < NUM_PRIO_LEVELS; j++) {
				// while the queue is not empty
				if (!is_empty(prio_queues[j])) {
					if (i < QUANTA_LIMIT || prio_queues[j]->head->process->start_time != -1) {
						active_process = dequeue(prio_queues[j]);
						if (active_process->start_time == -1)
							active_process->start_time = i;
					} else {
						dequeue(prio_queues[j]);
					}
					break;
				}
			}
		}
	}


	// *****CALCULATE/PRINT STATS******
	// process details
	//process_struct* ps;
	for (int i = 0; i < TOTAL_PROCESSES-1; i++) {
		ps = &processes[i];
		printf("Process name: %hhu\n", ps->process_name);
		printf("Arrival Time: %f\n", ps->arrival_time);
		printf("Expected Run Time: %f\n", ps->service_time);
		printf("Priority: %i\n\n", ps->priority);
	}

	// process record
	printf("\nPROCESS RECORD: \n");
	for (int i = 0; i < QUANTA_LIMIT; i++) {
		printf("%hhu,", process_record[i]);
	} printf("\n\n");

	// process stats
	// [0] is for all queues combined
	int num_completed[NUM_PRIO_LEVELS+1] = {0};
	float total_turnaround_time[NUM_PRIO_LEVELS+1] = {0.0};
	float total_wait_time[NUM_PRIO_LEVELS+1] = {0.0};
	float total_response_time[NUM_PRIO_LEVELS+1] = {0.0};
	float max_completion_time[NUM_PRIO_LEVELS+1] = {0.0}; // for calculating process throughput
	int prio;
	for (q_node* node = completed_processes->head; node != NULL; node = node->next) {
		// store process in temp struct for readability
		ps = node->process;
		prio = ps->priority;
		
		// updates total values
		num_completed[prio]++;
		num_completed[0]++;

		total_turnaround_time[prio] += ps->turnaround_time;
		total_turnaround_time[0] += ps->turnaround_time;

		total_wait_time[prio] += ps->wait_time;
		total_wait_time[0] += ps->wait_time;

		total_response_time[prio] += ps->response_time;
		total_response_time[0] += ps->response_time;

		max_completion_time[prio] = max(max_completion_time[prio+1], ps->end_time);
		max_completion_time[0] = max(max_completion_time[0], ps->end_time);
	}

	int n;
	for (int i = 0; i < NUM_PRIO_LEVELS; i++) {
		n = num_completed[i+1];
		printf("Priority Queue %i\n", i+1);
		printf("=================\n");
		printf("Completed processes: %i\n", n);
		//printf("Final end time: %f\n", max_completion_time[i+1]);
		printf("Mean turnaround time: %f\n", total_turnaround_time[i+1]/n);
		printf("Mean wait time: %f\n", total_wait_time[i+1]/n);
		printf("Mean response time: %f\n", total_response_time[i+1]/n);
		printf("Mean throughput: %f\n", n/max_completion_time[i+1]);
		printf("\n");
	}
	n = num_completed[0];
	printf("Overall Statistics\n");
	printf("=================\n");
	printf("Completed processes: %i\n", n);
	//printf("Final end time: %f\n", max_completion_time[0]);
	printf("Mean turnaround time: %f\n", total_turnaround_time[0]/n);
	printf("Mean wait time: %f\n", total_wait_time[0]/n);
	printf("Mean response time: %f\n", total_response_time[0]/n);
	printf("Mean throughput: %f\n", n/max_completion_time[0]);
	printf("\n");

	// incrementing global variables for total avg
	total_turnaround += total_turnaround_time[0]/n;
	total_wait += total_wait_time[0]/n;
	total_response += total_response_time[0]/n;
	total_throughput += n/max_completion_time[0];

	// ******CLEANUP******
	free_workload(processes, TOTAL_PROCESSES-1);
	for (int i = 0; i < NUM_PRIO_LEVELS; i++) 
		destroy_queue(prio_queues[i]);
	destroy_queue(process_queue);
	destroy_queue(completed_processes);

	return;
}
