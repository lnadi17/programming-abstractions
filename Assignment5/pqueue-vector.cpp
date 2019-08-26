/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"
#include "vector.h"

VectorPriorityQueue::VectorPriorityQueue() {

}

VectorPriorityQueue::~VectorPriorityQueue() {

}

int VectorPriorityQueue::size() {
	return qVector.size();
}

bool VectorPriorityQueue::isEmpty() {
	return qVector.isEmpty();
}

void VectorPriorityQueue::enqueue(string value) {
	qVector.add(value);
}

string VectorPriorityQueue::peek() {
	if (isEmpty()) error("Attempting to peek in an empty queue");
	return qVector.get(findMinIndex(qVector));
}

string VectorPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Attempting to dequeue an element from an empty queue");
	int minIndex = findMinIndex(qVector);
	string minElement = qVector.get(minIndex);
	qVector.remove(minIndex);
	return minElement;
}

/* Returns the index of lexicographically minimum element in passed vector */
int VectorPriorityQueue::findMinIndex(Vector<string> & vec) {
	int minIndex = 0;
	for(int i = 0; i < vec.size(); i++) {
		if (vec[minIndex] > vec[i]) {
			minIndex = i;
		}
	}
	return minIndex;
}
