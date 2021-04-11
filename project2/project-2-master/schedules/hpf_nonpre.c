#include "../process.h"
#include "../workload.h"
#include "../queue.h"
#include "hpf_nonpre.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void run_hpf_nonpre() {
	int num_processes = 100;
	char process_record[PROCESS_RECORD_LENGTH];

	// ******* SETUP *********
	// initialize process record
	// 255 is reserved for no processes
	for (int i = 0; i < PROCESS_RECORD_LENGTH; i++) {
		process_record[i] = (char)255;
	}

	// generate sorted array of process_struct s
	// TRY NOT TO GENERATE MORE THAN 255 process_structs(char limitations)
	process_struct* processes = create_workload(num_processes);
	// change the process names to numbers
	// creates a queue of processes by arrival time
	queue* process_queue = init_queue();
	for (int i = 0; i < num_processes; i++) {
		processes[i].process_name = (char)(i%256);
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
	// keeps running while within time limit/a process is still running
	for (int i = 0; i < QUANTA_LIMIT || (active_process != NULL); i++) {
		max_quanta = i + 1;  // saves total quanta run for

		// (1)
		// checks is current active_process has ended
		if (active_process != NULL) {
			//printf("active_process id: %hhu\n", (int)active_process->process_name);
			// checks is active process has ended
			if (active_process->end_time < i) {
				// adds active_process to completed_processes then marks it as completed
				enqueue(completed_processes, active_process); 
				active_process = NULL;
			}
			else // if active_process hasn't ended yet, update process record at this quantum
				process_record[i] = active_process->process_name;
		}
		//else
		//	printf("active_process id: NULL\n");

		// (2)
		// updates priority queues at each quanta
		// while process_queue is not empty and first process has already arrived
		float new_arrival_time;
		while(!is_empty(process_queue) && (process_queue->head->process->arrival_time < i)) {
			p = dequeue(process_queue); // dequeues from process queue
			new_arrival_time = p->arrival_time;

			// ***** AGING *******
			// update priority queues due to aging
			// do this before enqueueing each new process
			for (int j = 1; j < NUM_PRIO_LEVELS; j++) {
				// if process at head of priority queue is supposed to age-out before new process is enqueued
				while (!is_empty(prio_queues[j]) && prio_queues[j]->head->process->enqueue_time + AGE_OUT_TIME < new_arrival_time) {
					// moves process up a priority queue
					head_process = dequeue(prio_queues[j]);
					enqueue(prio_queues[j-1], head_process);
					head_process->enqueue_time += AGE_OUT_TIME; // update enqueue_time
				}
			}
			enqueue(prio_queues[p->priority-1], p); // enqueues into correct prio queue
			p->enqueue_time = p->arrival_time;
		}

		// (3)
		// check if need to dequeue/start new task
		if ((active_process == NULL) && (i < QUANTA_LIMIT)) {  
			// scans each priority queues until finds first available process
			// sets the process as the active process
			for (int j = 0; j < NUM_PRIO_LEVELS; j++) {
				if (!is_empty(prio_queues[j])) {
					active_process = dequeue(prio_queues[j]);
					process_record[i] = active_process->process_name;
					active_process->start_queue = j+1;
					active_process->start_time = i;
					active_process->end_time = ceil(i + active_process->service_time);

					active_process->turnaround_time = active_process->end_time - 
													active_process->arrival_time;
					active_process->wait_time = active_process->turnaround_time - 
													active_process->service_time;
					active_process->response_time = active_process->start_time - 
													active_process->arrival_time;
					break;
				}
			}
		}
	}




	// *****CALCULATE/PRINT STATS******
	// process details
	process_struct* ps;
	for (int i = 0; i < num_processes; i++) {
		ps = &processes[i];
		printf("Process name: %hhu\n", ps->process_name);
		printf("Arrival Time: %f\n", ps->arrival_time);
		printf("Expected Runtime: %f\n", ps->service_time);
		printf("Priority: %i\n\n", ps->priority);
	}

	// process record
	printf("\nPROCESS RECORD: \n");
	for (int i = 0; i < max_quanta-1; i++) {
		printf("%hhu,", process_record[i]);
	} printf("\n\n");

	/*for(q_node* n = completed_processes->head; n != NULL; n = n->next) {
		ps = n->process;
		printf("PROCESS: %hhu\n", ps->process_name);
		printf("process arrival_time: %f\n", ps->arrival_time);
		printf("turnaround_time: %f\n", ps->turnaround_time);
		printf("wait_time: %f\n", ps->wait_time);
		printf("start_time: %i\n", ps->start_time);
		printf("end_time: %i\n", ps->end_time);
		printf("service_time: %f\n", ps->service_time);
		printf("priority: %i\n\n", ps->priority);
	}*/

	// process stats
	// [0] is for all queues combined
	int num_completed[NUM_PRIO_LEVELS+1] = {0,0,0,0,0}; // not the best line of code
	float total_turnaround_time[NUM_PRIO_LEVELS+1];
	float total_wait_time[NUM_PRIO_LEVELS+1];
	float total_response_time[NUM_PRIO_LEVELS+1];
	float max_completion_time[NUM_PRIO_LEVELS+1]; // for calculating process throughput
	int prio, sq;
	for (q_node* node = completed_processes->head; node != NULL; node = node->next) {
		// store process in temp struct for readability
		ps = node->process;
		prio = ps->priority;
		sq = ps->start_queue;
		
		// updates total values
		num_completed[sq]++;
		num_completed[0]++;

		total_turnaround_time[sq] += ps->turnaround_time;
		total_turnaround_time[0] += ps->turnaround_time;

		total_wait_time[sq] += ps->wait_time;
		total_wait_time[0] += ps->wait_time;

		total_response_time[sq] += ps->response_time;
		total_response_time[0] += ps->response_time;

		max_completion_time[sq] = max(max_completion_time[sq], ps->end_time);
		max_completion_time[0] = max(max_completion_time[0], ps->end_time);
	}

	int n;
	for (int i = 0; i < NUM_PRIO_LEVELS; i++) {
		n = num_completed[i+1];
		printf("Priority Queue %i\n", i+1);
		printf("=================\n");
		printf("Completed processes: %i\n", n);
		printf("Final end time: %f\n", max_completion_time[i+1]);
		printf("Mean turnaround time: %f\n", total_turnaround_time[i+1]/n);
		printf("Mean wait time: %f\n", total_wait_time[i+1]/n);
		printf("Mean response time: %f\n", total_response_time[i+1]/n);
		printf("Mean throughput: %f\n", n/max_completion_time[0]);
		printf("\n");
	}
	n = num_completed[0];
	printf("Overall Statistics\n");
	printf("=================\n");
	printf("Completed processes: %i\n", n);
	printf("Final end time: %f\n", max_completion_time[0]);
	printf("Mean turnaround time: %f\n", total_turnaround_time[0]/n);
	printf("Mean wait time: %f\n", total_wait_time[0]/n);
	printf("Mean response time: %f\n", total_response_time[0]/n);
	printf("Mean throughput: %f\n", n/max_completion_time[0]);
	printf("\n");



	// ******CLEANUP******
	free_workload(processes, num_processes);
	for (int i = 0; i < NUM_PRIO_LEVELS; i++) 
		destroy_queue(prio_queues[i]);
	destroy_queue(process_queue);
	destroy_queue(completed_processes);
}



void test_queues(void) {

	process_struct ps1, ps2, ps3;
	ps1.arrival_time = 1;
	ps1.service_time = 1;
	ps1.priority = 1;

	ps2.arrival_time = 2;
	ps2.service_time = 2;
	ps2.priority = 2;

	ps3.arrival_time = 3;
	ps3.service_time = 3;
	ps3.priority = 3;

	queue* q = init_queue();

	process_struct* should_be_null = dequeue(q);
	if(should_be_null == NULL)
		printf("dequeue empty pass\n");
	else
		printf("dequeue empty fail\n");

	enqueue(q, &ps1);
	enqueue(q, &ps2);
	enqueue(q, &ps3);

	if (destroy_queue(q))
		printf("destroy_queue when it shouldn't have : bad!\n");
	else 
		printf("preserved non-empty queue : good!\n");

	process_struct* ps1_2 = dequeue(q);
	process_struct* ps2_2 = dequeue(q);
	printf("dequeued %i, %i\n", ps1_2->priority, ps2_2->priority);


	process_struct* ps3_2 = dequeue(q); 
	if (is_empty(q)) 
		printf("queue is now empty!! - good!\n");
	else
		printf("queue is not considered empty - bad!!\n");

	enqueue(q, ps1_2);
	if (is_empty(q)) 
		printf("queue is empty - should not be!!\n");
	else
		printf("queue is not empty - as it should be\n");
}

float max(float a, float b) {
	return (a > b) ? a : b;
}

