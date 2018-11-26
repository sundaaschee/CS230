#ifndef __PQUEUE_H
#define __PQUEUE_H

#include "tree.h"

/**
 * Represents a priority queue.
 */
typedef struct PriorityQueue PriorityQueue;


/**
 * Creates a new PriorityQueue object.
 */
PriorityQueue *pqueue_new ();


/**
 * Deallocates a priority queue.
 */
void pqueue_free (PriorityQueue *pq);


/**
 * Enqueues the given TreeNode object `n` in the PriorityQueue `pq`.
 */
void pqueue_enqueue (PriorityQueue *pq, TreeNode *n);


/**
 * Dequeues the TreeNode object in the PriorityQueue.  Returns NULL if the
 * PriorityQueue is empty.
 */
TreeNode *pqueue_dequeue (PriorityQueue *pq);


/**
 * Returns the size of the priority queue.
 */
int pqueue_size (PriorityQueue *pq);


/**
 * Prints the priority queue.
 */
void pqueue_print (PriorityQueue *pq);

#endif
