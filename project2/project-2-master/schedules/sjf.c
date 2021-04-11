#include "../process.h"
#include "../workload.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

float sjf_TAT=0;
float sjf_W=0;
float sjf_R=0;

void run_sjf() {

	// Running 5 times
	run_one_sjf();
	run_one_sjf();
	run_one_sjf();
	run_one_sjf();
	run_one_sjf();
	printf("Overall turnaround time: \n\t%.2f\nOverall waiting time: \n\t%.2f\nOverall response time: \n\t%.2f\n",sjf_TAT/5,sjf_W/5,sjf_R/5);
}

void run_one_sjf(){ //creating 60 processes 
	int num_processes = 60;
	process_struct* workload = create_workload(num_processes);
	print_workload(workload,num_processes);
	
	
	//lots of variable intialization
	char current_process = '-';
	int current_arrival = -1;
	int current_service = -1;
	int count=0;
	int tchart=0;
	int ctr=0;
	
	char timechart[200] = {[0 ... 199]='-'};
	int turnaround_time=0;
	int response_time=0;
	int waiting_time=0;

	int issued_processes=0;
	int current;
	int start;
	int quantum =0;
	float cur_task_serv = -2;
	for (int i=0;i<100;++i) 
	{
		quantum=i+1;
		start=i+1;
		cur_task_serv = -2;
		
		for(size_t j=0;j<num_processes;++j) //picking the job with the shortest service time
		{
			if(workload[j].arrival_time > quantum) //arrives after current quantum
				continue;
			if(workload[j].start_time!=-1)
				continue;
			if(cur_task_serv == -2  || workload[j].service_time < cur_task_serv)
			{
				current = j;
				cur_task_serv=workload[j].service_time;
			}
		}
			
		if(workload[current].start_time == -1) //if current process hasnt been serviced
		{
			current_process = workload[current].process_name;
			current_arrival = workload[current].arrival_time;
			current_service = workload[current].service_time;
		
			timechart[tchart]=current_process;
			tchart+=1;	

			workload[current].start_time=start;
	
			int cur_turnaround = quantum-current_arrival+current_service;
			int cur_response = quantum-current_arrival;
			int cur_waiting = cur_turnaround - current_service;
	
			turnaround_time += cur_turnaround;
			response_time += cur_response;
			waiting_time += cur_waiting;

			printf("Process: %c \n\tQuantum: %d\n\tStart Time: %d\n \tTurnaround: %d \n \tResponse:  %d \n\tWaiting: %d \n\tCurrent Arrival: %d\n", current_process, quantum, start,cur_turnaround,cur_response,cur_waiting,current_arrival);
		if(i+current_service<100) //only count processes fully serviced by end of quantum 99
		{
			i+=current_service;
			ctr+=1;
		}
		}
	
	}

	float avg_turnaround = turnaround_time / (float) num_processes; //calculating stats
	float avg_waiting = waiting_time / (float) num_processes;
	float avg_response = response_time / (float) num_processes;
	float avg_throughput = ctr/ (float) quantum * 100;

	sjf_TAT+=avg_turnaround;
	sjf_W+=avg_waiting;
	sjf_R+=avg_response;

	printf("Average turnaround: %.2f\n Average waiting: %.2f\n Average response: %.2f\n Average throughput: %.2f\n",avg_turnaround, avg_waiting, avg_response, avg_throughput);
	printf("Timechart:\n");
	putchar('\t');
	for(int i = 0; i<200;++i)
	{
		if(timechart[i]!='-')
			putchar(timechart[i]);
	}
	putchar('\n');
	free_workload(workload, num_processes);
	
}
