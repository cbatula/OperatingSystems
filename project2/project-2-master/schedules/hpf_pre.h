#ifndef HPF_PRE_H
#define HPF_PRE_H
#include "../process.h"
#include "../workload.h"
#include "../queue.h"

#define NUM_PRIO_LEVELS 4
#define TOTAL_PROCESSES 100
#define QUANTA_LIMIT 100
#define PROCESS_RECORD_LENGTH 1000
#define AGE_OUT_TIME 5

void run_hpf_pre();

void hpf_pre();


#endif
