#ifndef HPF_NP_H
#define HPF_NP_H
#include "../process.h"
#include "../workload.h"
#include "../queue.h"


#define NUM_PRIO_LEVELS 4
#define AGE_OUT_TIME 5
#define QUANTA_LIMIT 100
#define PROCESS_RECORD_LENGTH 110


void run_hpf_nonpre(void);

void test_queues(void);

float max(float a, float b);

#endif
