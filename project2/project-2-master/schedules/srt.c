#include "../process.h"
#include "../workload.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define RUN_COUNT 5
#define PROCESS_COUNT 40

#define MAX(a, b) ((a) > (b) ? (a) : (b))

process_struct* select_shortest_task(process_struct* workload, size_t current_quanta) {
    process_struct* shortest = NULL;
    for (size_t i = 0; i < PROCESS_COUNT; i++) {

        // Skip tasks that have not arrived
        if (workload[i].arrival_time > current_quanta) {
            continue;
        }

        // Skip completed tasks
        if (workload[i].end_time != -1) {
            continue;
        }

        // Update shortest to current located shortest task
        if (shortest == NULL || workload[i].service_time < shortest->service_time) {
            shortest = &workload[i];
        }
    }
    
    return shortest;
}

bool complete(process_struct* workload, size_t i) {
    if(i < 100) {
        return false;
    }

    bool completed = true;
    // check for processes that have started but not finished
    for (size_t i = 0; i < PROCESS_COUNT; i++) {
        if (workload[i].start_time != -1 && workload[i].end_time == -1) {
            completed = false;
        }
    }

    return completed;
}

void update_wait_times(process_struct* workload, process_struct* current, size_t quanta) {
    for (size_t i = 0; i < PROCESS_COUNT; i++) {
        // if it wasn't run
        if (&workload[i] == current) {
            continue;
        }

        // if it has arrived
        if (workload[i].arrival_time > quanta) {
            continue;
        }

        // if it hasn't finished    
        if (workload[i].end_time != -1) {
            continue;
        }

        workload[i].wait_time += 1;
    }
    
}

size_t processes_run(process_struct* workload) {
    size_t count = 0;
    for (size_t i = 0; i < PROCESS_COUNT; i++) {
        if (workload[i].start_time != -1) {
            count++;
        }
    }
    return count;
}

void run_one_srt() {
    // creates the workload
    process_struct* workload = create_workload(PROCESS_COUNT);
    print_workload(workload, PROCESS_COUNT);

    // run until we have finished everything we started or 100 quanta, 
    // whichever comes first
    size_t i;
    for (i = 0; !complete(workload, i); i++) {
        process_struct* to_run = select_shortest_task(workload, i);
        if (to_run == NULL) {
            putchar(' ');
            continue;
        } else {
            putchar(to_run->process_name);
        }

        update_wait_times(workload, to_run, i);

        to_run->service_time = MAX(to_run->service_time - 1.0, 0);
        if (to_run->start_time == -1) {
            to_run->start_time = i;
        }
        if (to_run->service_time <= 0) {
            to_run->end_time = i;
        }
    }
    putchar('\n');

    // calculate results
    float average_tat = 0;
    float average_wait = 0;
    float average_response = 0;

    for (size_t i = 0; i < PROCESS_COUNT; i++) {
        process_struct* task = &workload[i];
        if (task->start_time == -1) {
            continue;
        }

        average_tat += (task->end_time - task->arrival_time);
        average_wait += task->wait_time;
        average_response += (task->start_time - task->arrival_time);
    }

    average_tat = average_tat / (float) PROCESS_COUNT;
    average_wait = average_wait / (float) PROCESS_COUNT;
    average_response = average_response / (float) PROCESS_COUNT;

    printf("Average turnaround: %.2f\n", average_tat);
    printf("Average response: %.2f\n", average_response);
    printf("Average waiting: %.2f\n", average_wait);

    float throughput = ((float) processes_run(workload) / i) * 100;
    printf("throughput: %.2f per 100 quantum\n", throughput);
    
    free_workload(workload, PROCESS_COUNT);
}

void run_srt() {
    for (size_t i = 0; i < RUN_COUNT; i++) {
        printf("=====SRT run %lu================\n", i);
        run_one_srt();
    }

    // for (size_t i = 0; i < 100; i++)
    // {
    //     putchar('0');
    // }
    // putchar('\n');
}

