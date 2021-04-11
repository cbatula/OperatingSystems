#include "process.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

char process_names[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
int processes_created = 0;
int overflow = 0;

char get_name() {
    // returns a character from the array
    return process_names[processes_created];
}

void seed_process_random(char c) {
    // change this when we want real random
    // srand(c* 1000 + overflow + time(NULL));
    srand(c* 1000 + overflow);
}

float get_arrival_time() {
    // returns a random value [0, 99]
    return ((float)rand()/RAND_MAX)*99; 

}

float get_service_time() {
    // returns a random value [0.1, 10.0]
    return ((float)rand()/RAND_MAX)*9.9 + 0.1;
}

int get_priority() {
    // returns a priority
    int priority = (rand() % 4) + 1;
    processes_created += 1;
    if (processes_created > 60) {
        // if we create more than 61 processes, then it starts naming them from A again
        overflow = 26000 + overflow;
        processes_created = 0;
    }
    return priority;
}


// process_struct* create_process() {
//     // create random times for new process
//     // printf("Creating process %d\n",processes_created);
//     // srand(processes_created* 1000);
//     // int arrival_time = rand() % 100;
//     // int service_time = (rand() %11);
//     // if (service_time == 0) {
//     //     service_time += 1;
//     // }
//     // int priority = rand() % 5;
//     // if (priority == 0) {
//     //     priority += 1;
//     // }
//     // char process_name = process_names[processes_created];
//     // processes_created += 1;
//
//     // create a new process
//     process_struct* new_struct;
//     new_struct = malloc(sizeof(process_struct));
//     new_struct->process_name = process_name;
//     new_struct->arrival_time = arrival_time;
//     new_struct->service_time = service_time;
//     new_struct->priority = priority;
//     new_struct->turnaround_time = 0;
//     new_struct->response_time = 0;
//     new_struct->wait_time = 0;
//     return new_struct;
// }
//
// void free_process(process_struct* process_s) {
//     free(process_s);
// }
//
// void print_process(process_struct* process) {
//   printf("name %c, "
//          "arrival %d, "
//          "service %d, "
//          "priority %d, "
//          "turnaround %d, "
//          "response %d, "
//          "wait %d\n",
//          process->process_name,
//          process->arrival_time,
//          process->service_time,
//          process->priority,
//          process->turnaround_time,
//          process->response_time,
//          process->wait_time);
// }
