
/**
 *	Water Retention on Magic Squares Solver
 *
 *	Author: Johan Öfverstedt
 *	Modified: July 2012
 *	Version 0.12a
 *
 *	minpriorityqueue.h
 *	Minimum Priority Queue Implementation
 *
 *	Project website:
 *	sourceforge.net/projects/wrmssolver
 *
 *	Based on thesis:
 *	http://urn.kb.se/resolve?urn=urn:nbn:se:uu:diva-176018
 *
 */
 
#ifndef _MINPRIORITY_QUEUE_H_
#define _MINPRIORITY_QUEUE_H_

//Experiments indicate the more advanced minheap implementation
//is actually slower.
//#define MINHEAP_QUEUE_IMPLEMENTATION

class MinPriorityQueue {
public:
	MinPriorityQueue(int param_capacity);
	~MinPriorityQueue();

	int size() { return used; }

	void enqueue(int param_index, int param_value);
	bool dequeue(int *param_index_out, int *param_value_out);


#ifdef MINHEAP_QUEUE_IMPLEMENTATION
	void minheapify(int param_index);
#endif

protected:
	int capacity;
	int used;

	int *index_queue;
	int *value_queue;
};

#endif