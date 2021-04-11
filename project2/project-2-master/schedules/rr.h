#ifndef RR_H
#define RR_H
#include "../process.h"
#include "../workload.h"
#include "../queue.h"

#define TOTAL_TASKS 30
#define TOTAL_QUANTA 100
#define TIME_SLICE 1

//Functions runs all 5 passes
void run_rr();

//Function runs one single pass
void rr_single(process_struct *sorted_processes);

//int move_end(process_struct *sorted_processes, float quanta, int end);

//Print completed process
void print_completed_processes(process_struct *sorted_processes);

#endif
