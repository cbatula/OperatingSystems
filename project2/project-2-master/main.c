#include "process.h"
#include "workload.h"
#include "run.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [scheduler id]\n", argv[0]);
        return 1;
    }

    run_schedule(atoi(argv[1]));

    return 0;
}
