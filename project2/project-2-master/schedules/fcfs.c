#include "../process.h"
#include "../workload.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void run_fcfs() {
  printf("=====FCFS run 1================\n");
  run_one_fcfs();
  printf("=====FCFS run 2================\n");
  run_one_fcfs();
  printf("=====FCFS run 3================\n");
  run_one_fcfs();
  printf("=====FCFS run 4================\n");
  run_one_fcfs();
  printf("=====FCFS run 5================\n");
  run_one_fcfs();
}

void run_one_fcfs() {
    // sets number of processes to create
    int num_processes = 60;
    printf("In fcfs\n");
    // creates the workload
    process_struct* workload = create_workload(num_processes);
    print_workload(workload, num_processes);

    // creates the timechart and initializes every quantum to idle
    char time_chart[200] = {[0 ... 199] = '0'};
    int process_num = 0;

    // gets the first process in the workload
    char current_process_name = workload[process_num].process_name;
    int current_arr_time = workload[process_num].arrival_time;
    int current_serv_time = workload[process_num].service_time;
    //printf("current process name: %c, current arr time: %d, current serv time: %d\n",
    //        current_process_name, current_arr_time, current_serv_time);
    int total_turnaround = 0;
    int total_response = 0;
    int total_waiting = 0;

    int quantum;
    // for each quantum
    for (int i = 0; i < 100; i++) {
        quantum = i + 1;
        //printf("quantum: %d\n", quantum);
        // if it is past the arrival time of the next process, handle it
        if (quantum >= current_arr_time && process_num < num_processes) {
            //printf("starting process %c\n", current_process_name);
            int start_time = quantum;
            printf("start time: %d\n", start_time);
            //printf("time chart: %.200s\n", time_chart);
            for (int j = i; j < start_time-1 + current_serv_time; j++,i++) {
                time_chart[j] = current_process_name;
            }
            i--;
            quantum = i + 1;

            // calculate numbers
            int turnaround_time = quantum - current_arr_time + 1;
            int response_time = start_time - current_arr_time;
            int waiting_time = response_time;
            printf("process %c, turnaround %d, response %d, waiting %d\n",
                   current_process_name, turnaround_time, response_time, waiting_time);
            total_turnaround = total_turnaround + turnaround_time;
            total_response = total_response + response_time;
            total_waiting = total_waiting + waiting_time;
            // increment number of processes handled, and get the next one
            process_num = process_num + 1;
            //printf("process num: %d, num process: %d\n", process_num, num_processes);
            if (process_num < num_processes) {
                //printf("updating to next process\n");
                current_process_name = workload[process_num].process_name;
                current_arr_time = workload[process_num].arrival_time;
                current_serv_time = workload[process_num].service_time;
                //printf("new stuff: %c, %d, %d\n",
                //       current_process_name, current_arr_time, current_serv_time);
            }
        }
    }
    // get average numbers of the workload
    float average_turnaround = total_turnaround / (float)process_num;
    float average_response = total_response / (float)process_num;
    float average_waiting = total_waiting / (float)process_num;
    printf("Average turnaround: %.2f\n", average_turnaround);
    printf("Average response: %.2f\n", average_response);
    printf("Average waiting: %.2f\n", average_waiting);
    float throughput = process_num/(float)quantum * 100;
    printf("throughput: %.2f per 100 quantum\n", throughput);
    // print the time chart
    for (int k = 0; k < 200; k++) {
      printf("%c", time_chart[k]);
      if ((k+1) % 10 == 0) {
        printf(" ");
      }
    }
    printf("\n");
    //free the memory
    free_workload(workload, num_processes);
}
