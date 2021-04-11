#!/bin/bash

# runs gcc with everything
gcc -g main.c process.c run.c workload.c schedules/fcfs.c schedules/hpf_nonpre.c schedules/hpf_pre.c schedules/rr.c schedules/sjf.c schedules/srt.c queue.c  -lm
