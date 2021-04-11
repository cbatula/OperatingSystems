#include "rr.h"
#include <string.h>
#include <stdlib.h>

//Numbers for the average across all runs
float all_tat=0;
float all_resp=0;
float all_wt=0;
float all_through=0;

//This function runs all 5 passes
void run_rr() 
{
  
  int i;
  float total_throughput;
  process_struct *processes;

  for (i=0 ; i<5 ; ++i)
  {
    printf("<===ROUND ROBIN RUN %d===>\n",i+1);
    processes = create_workload(TOTAL_TASKS);
    print_workload(processes, TOTAL_TASKS);
    printf("\n");
    rr_single(processes);
    free_workload(processes, TOTAL_TASKS);
    printf("\n\n"); 
  }
  //Print averages across all 5 runs
  printf("Average Throughput of all runs: %f\n", all_through/i);
  printf("Average Turnaround Time of all runs: %f\n", all_tat/i);
  printf("Average Wait Time of all runs: %f\n", all_wt/i); 
  printf("Average Response Time of all runs: %f\n", all_resp/i); 
  
}

void rr_single(process_struct *sorted_processes)
{
  //Declare and Initialize Variables
  int next_process;
  queue *myqueue = init_queue();
  process_struct *original_processes, *working_process, *unmodified_process;
  float current_quanta, activated, throughput, total_turnaround, total_wait, total_response;
  float executed_time[TOTAL_TASKS] = {0.0};
  float service_times[TOTAL_QUANTA*10] = {0.0};
  current_quanta = activated = throughput = total_turnaround = total_wait = total_response = 0;
  next_process = 0;
  unmodified_process=working_process= NULL;
  original_processes = malloc(sizeof(process_struct) * TOTAL_TASKS);

  //Traverse through quanta 0 to 99
  //for ( current_quanta ; current_quanta < TOTAL_QUANTA ; current_quanta+=1.0 )
  while((current_quanta<TOTAL_QUANTA) || (!is_empty(myqueue)) )
  {
    
    //Move ending bound of viable options
    while((next_process<TOTAL_TASKS)&&(sorted_processes[next_process].arrival_time <= current_quanta)&&(current_quanta<TOTAL_QUANTA))
    {
      enqueue(myqueue, &sorted_processes[next_process]);
      //enqueue(unmodified_queue, &original_processes[next_process]);
      next_process++;
    }

    //Add process back to queue if not done
    if(working_process!=NULL)
    {
      enqueue(myqueue, working_process);
      //enqueue(unmodified_queue, unmodified_process);
    }

    //If no viable options to work on, continue
    if (is_empty(myqueue))
    {
      //Exit loop if no more tasks to do
      if(current_quanta > TOTAL_QUANTA)
        break;
      printf("Quanta %d: Idle\n", (int)current_quanta);
      current_quanta++;
      continue;
    }
    //Remove process from the queue
    working_process = dequeue(myqueue);
    //unmodified_process = dequeue(unmodified_queue);
    service_times[(int)current_quanta]=working_process->service_time;

    //If the process hasn't started
    if(working_process->start_time == -1)
    {
	//If it is past the deadline, don't start the task
        if(current_quanta>=TOTAL_QUANTA)
	{
	  working_process=NULL;
	  continue;
	}
      //Set start time
      working_process->start_time = current_quanta;
      activated+=1.0;
      //Set response time and store for computation later
      working_process->response_time = (float)working_process->start_time - working_process->arrival_time;
      total_response += working_process->response_time;
    }

    //Decrement remaining service time
    working_process->service_time = working_process->service_time - 1; 
    printf("Quanta %d: %c\n", (int)current_quanta, working_process->process_name);

    //If task is done
    if(working_process->service_time <= 0)
    {
      //Retrieve initial start time
      working_process->service_time = service_times[working_process->start_time];
      throughput+=1.0;
      //printf("Current_Quanta Done: %f\n", current_quanta);
      working_process->end_time = current_quanta+1;
      //printf("Current_Quanta Done: %f\n", sorted_processes[p % (end - start)+start].end_time);        
      //Update specifics for the tasks
      working_process->turnaround_time = (float)working_process->end_time - working_process->arrival_time;
      working_process->wait_time = working_process->turnaround_time - working_process->service_time;
      //Add to the totals
      total_turnaround += working_process->turnaround_time;
      total_wait += working_process->wait_time;
      working_process=NULL;
    }
    current_quanta+=1.0;
  }
  //Store computed statistics related to the run
  float completed_tasks = throughput;
  throughput = throughput/current_quanta*100;
  float response = (float)total_response/completed_tasks;
  float wait = (float)total_wait/completed_tasks;
  float turnaround = (float)total_turnaround/completed_tasks;

  //Store statistics for all runs
  print_completed_processes(sorted_processes);
  all_through+=throughput;
  all_wt+=(float)wait;
  all_resp+=(float)response;
  all_tat+=(float)turnaround;

  //Print run specifc statistics
  printf("Throughput: %f\n", throughput);
  printf("Average Turnaround: %f\n", turnaround);
  printf("Average Wait: %f\n", wait);
  printf("Average Response: %f\n", response);
  return;
}

//Print statistics for any tasks that completed
void print_completed_processes(process_struct *sorted_processes)
{
  printf("\nCompleted Process Statistics\n");
  int i;
  for (i=0; i < TOTAL_TASKS; i++)
  {
    if(sorted_processes[i].end_time != (float)(-1))
    {
      printf("Process name: %c\n", sorted_processes[i].process_name);
      printf("Arrival Time: %f\n", sorted_processes[i].arrival_time);
      printf("Expected Runtime: %f\n", sorted_processes[i].service_time);
      printf("Priority: %d\n", sorted_processes[i].priority);
      printf("Start Time: %d\n", sorted_processes[i].start_time);
      printf("End Time: %d\n", sorted_processes[i].end_time);
      printf("Turnaround time: %f\n", sorted_processes[i].turnaround_time);
      printf("Response time: %f\n", sorted_processes[i].response_time);
      printf("Wait time: %f\n\n", sorted_processes[i].wait_time); 
    }
  }
}
