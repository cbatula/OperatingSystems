#ifndef QUEUE
#define QUEUE
#include "workload.h"

typedef struct q_node q_node;
typedef struct q_node {
	q_node* next;
	process_struct* process;
} q_node;


// linked-list based queue
typedef struct queue {
	q_node* head;
	q_node* tail;
} queue;

// initializes and returns an empty queue
queue* init_queue(void);

// enqueues a process_struct onto the queue
// mallocs a q_node struct
void enqueue(queue* q, process_struct* process);

// dequeues a process_struct from the queue
// frees the q_node struct
// returns NULL if empty
process_struct* dequeue(queue* q);

// returns 1 if queue is empty, 0 if not
int is_empty(queue* q);

// destroys queue no matter how many item still in there
// returns the number of remaining items
int destroy_queue(queue* q);


#endif
