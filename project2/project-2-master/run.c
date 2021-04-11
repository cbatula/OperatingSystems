#include "process.h"
#include "workload.h"
#include "schedules/fcfs.h"
#include "schedules/sjf.h"
#include "schedules/srt.h"
#include "schedules/rr.h"
#include "schedules/hpf_nonpre.h"
#include "schedules/hpf_pre.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void run_schedule(int schedule) {
  //determines which scheduler to run. Only has fcfs for now
  if (schedule == 1) {
      run_fcfs();
  }
  if (schedule == 2) {
      run_sjf();
  }
  if (schedule == 3) {
      run_srt();
  }
  if (schedule == 4) {
      run_rr();
  }
  if (schedule == 5) {
      run_hpf_nonpre();
  }
  if (schedule == 6) {
      run_hpf_pre();
  }
}
