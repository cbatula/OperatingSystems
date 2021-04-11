#include "process.h"
#include "workload.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// used when creating workload
int struct_comparator(process_struct* struct_1, process_struct* struct_2)
{
    if (struct_1->arrival_time < struct_2->arrival_time)
        return -1;
    else if (struct_1->arrival_time > struct_2->arrival_time)
        return +1;
    else
        return 0;
}

// creates a workload of many processes as an array of structs
process_struct* create_workload(int size) {
    process_struct* workload = malloc(sizeof(process_struct) * size);
    printf("Creating workload\n");
    for (int i = 0; i < size; i++) {
        workload[i].process_name = get_name();
        seed_process_random(workload[i].process_name);
        workload[i].arrival_time = get_arrival_time();
        workload[i].service_time = get_service_time();
        workload[i].priority = get_priority();
        workload[i].start_time = -1;
        workload[i].end_time = -1;
        workload[i].turnaround_time = 0;
        workload[i].response_time = 0;
        workload[i].wait_time = 0;
    }
    // sorts by arrival time
    qsort(workload, size, sizeof(workload[0]), struct_comparator);
    return workload;
}

// frees a workload given
void free_workload(process_struct* workload, int size) {
    printf("Freeing workload\n");
    free(workload);
}

// prints a workload
void print_workload(process_struct* workload, int size) {
  for (int i = 0; i < size; i++) {
    printf("name %c, "
           "arrival %f, "
           "service %f, "
           "priority %d, \n",
             workload[i].process_name,
             workload[i].arrival_time,
             workload[i].service_time,
             workload[i].priority);
  }
}
