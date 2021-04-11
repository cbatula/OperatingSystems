#include "process.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"
#include "workload.h"


// initializes and returns an empty queue
queue* init_queue(void) {
	queue* q = (queue*)malloc(sizeof(queue));
	q->head = NULL;
	q->tail = NULL;
	return q;
}

// enqueues a process_struct* onto the end of the queue
// mallocs a q_node struct 
void enqueue(queue* q, process_struct* process) {
	// creates a q_node 
	q_node* node = (q_node*)(malloc(sizeof(q_node)));
	node->next = NULL; // is end of queue
	node->process = process;  // places process into q_node

	if (is_empty(q)) { // checks for empty queue
		// adds node as head and tail of q
		q->head = node;
		q->tail = node;
	}	
	else { // queue is not empty
		q->tail->next = node; // attaches node to the end of the queue
		q->tail = node; // assigns node as the new tail
	}
}

// dequeues a process_struct* from the queue
// q_node memory is automatically freed
// returns the process_struct* at the head of the queue
// returns NULL if queue was empty
process_struct* dequeue(queue* q) {
	if (is_empty(q)) // checks for empty queue
		return NULL;

	q_node* old_head = q->head; // stores old head pointer
	q->head = q->head->next; // moves head pointer to the next process in queue

	// retrieves process_struct* and frees q_node's memory
	process_struct* dequeued_process = old_head->process; // stores process_struct pointer
	free(old_head); // frees old queue head memory
	return dequeued_process;
}

// returns 1 if queue is empty, 0 if not
int is_empty(queue* q) {
	if (q->head == NULL)
		return 1;
	return 0;
}

// destroys queue no matter how many items are still in there
// returns the number of remaining items
int destroy_queue(queue* q) {
	int num_left = 0;
	while (!is_empty(q)){
		num_left++;
		dequeue(q);
	}
	free(q);
	return num_left;
}
