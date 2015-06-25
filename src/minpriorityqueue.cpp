
/**
 *	Water Retention on Magic Squares Solver
 *
 *	Author: Johan Öfverstedt
 *	Modified: July 2012
 *	Version 0.12a
 *
 *	minpriorityqueue.cpp
 *	Minimum Priority Queue Implementation
 *
 *	Project website:
 *	sourceforge.net/projects/wrmssolver
 *
 *	Based on thesis:
 *	http://urn.kb.se/resolve?urn=urn:nbn:se:uu:diva-176018
 *
 */
 
#include "minpriorityqueue.h"

MinPriorityQueue::MinPriorityQueue(int param_capacity) {

	capacity = param_capacity;
	used = 0;

	index_queue = new int[capacity];
	value_queue = new int[capacity];

}

MinPriorityQueue::~MinPriorityQueue() {

	if(index_queue)
		delete[] index_queue;
	if(value_queue)
		delete[] value_queue;

}

#ifndef MINHEAP_QUEUE_IMPLEMENTATION

void MinPriorityQueue::enqueue(int param_index, int param_value) {

	int i = used - 1;
	for(; i >= 0 && value_queue[i] < param_value; --i) {
		index_queue[i+1] = index_queue[i];
		value_queue[i+1] = value_queue[i];
	}
	index_queue[i+1] = param_index;
	value_queue[i+1] = param_value;

	++used;
}

bool MinPriorityQueue::dequeue(int *param_index_out, int *param_value_out) {
	
	if(used < 1)
		return false;

	--used;

	(*param_index_out) = index_queue[used];
	(*param_value_out) = value_queue[used];

	return true;

}

#else


void MinPriorityQueue::enqueue(int param_index, int param_value) {

	index_queue[used] = param_index;
	value_queue[used] = param_value;

	int i = used;
	int parent = ((i + 1) >> 1) - 1;
	while(i > 0 && value_queue[parent] > param_value) {
		int index_tmp = index_queue[i];
		index_queue[i] = index_queue[parent];
		index_queue[parent] = index_tmp;
		int value_tmp = value_queue[i];
		value_queue[i] = value_queue[parent];
		value_queue[parent] = value_tmp;
		
		i = parent;
		parent = ((i + 1) >> 1) - 1;
	}

	++used;

}

bool MinPriorityQueue::dequeue(int *param_index_out, int *param_value_out) {
	
	if(used < 1)
		return false;

	(*param_index_out) = index_queue[0];
	(*param_value_out) = value_queue[0];

	index_queue[0] = index_queue[used - 1];
	value_queue[0] = value_queue[used - 1];

	--used;

	minheapify(0);

	return true;

}

void MinPriorityQueue::minheapify(int param_index) {

	int left = 2 * (param_index + 1) - 1;
	int right = left + 1;

//	int parent = ((param_index + 1) / 2) - 1;

	int smallest_index = param_index;
	if(left < used && value_queue[left] < value_queue[param_index])
		smallest_index = left;
	if(right < used && value_queue[right] < value_queue[smallest_index])
		smallest_index = right;
	if(smallest_index != param_index) {
		int index_tmp = index_queue[param_index];
		index_queue[param_index] = index_queue[smallest_index];
		index_queue[smallest_index] = index_tmp;
		int value_tmp = value_queue[param_index];
		value_queue[param_index] = value_queue[smallest_index];
		value_queue[smallest_index] = value_tmp;

		minheapify(smallest_index);
	}
}


#endif

