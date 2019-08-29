/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "pqueue-heap.h"
#include "error.h"

HeapPriorityQueue::HeapPriorityQueue() {
	capacity = INITIAL_CAPACITY;
	arr = new string[capacity];
	count = 0;
}

HeapPriorityQueue::~HeapPriorityQueue() {
	delete[] arr;
	count = 0;
	capacity = 0;
}

int HeapPriorityQueue::size() {
	return count;
}

bool HeapPriorityQueue::isEmpty() {
	return count == 0;
}

void HeapPriorityQueue::enqueue(string value) {
	if (count == capacity) extendCapacity();
	count++;
	// Add value in an array
	arr[count - 1] = value;
	// Recursively do bubble-up starting from the last element
	bubbleUp(count - 1);
}

string HeapPriorityQueue::peek() {
	if (isEmpty()) error("Array is empty");
	return arr[0];
}

string HeapPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Array is empty");
	string result = arr[0];
	// Before returning the result, we should clean up the tree

	// Swap first and last values
	arr[0] = arr[count - 1];
	// Delete min (maybe it's not neccessary?)
	// delete arr[count - 1];

	// Bubble down the last value
	if (--count != 0) {
		bubbleDown(0);
	}
	return result;
}

void HeapPriorityQueue::extendCapacity() {
	capacity *= 2;
	string *oldArr = arr;
	arr = new string[capacity];
	for (int i = 0; i < count; i++) {
		arr[i] = oldArr[i];
	}
	delete[] oldArr;
}

void HeapPriorityQueue::bubbleUp(int index) {
	// Base case
	if (index == 0) return;

	string child = arr[index];
	string parent = arr[(index - 1) / 2];

	if (child < parent) {
		// Child and parent swap locations
		arr[index] = parent;
		arr[(index - 1) / 2] = child;

		// Bubble-up continues
		bubbleUp((index - 1) / 2);
	}
}

void HeapPriorityQueue::bubbleDown(int index) {
	// If at least one child exists, swap current node with the smaller child
	if (2 * index + 1 < count) {
		int childIndex = 2 * index + 1;
		// Checks if the second child exists and is smaller than the first one
		if (childIndex + 1 < count && arr[childIndex + 1] < arr[childIndex]) {
			childIndex++;
		}
		if (arr[childIndex] < arr[index]) {
			string tmp = arr[childIndex];
			arr[childIndex] = arr[index];
			arr[index] = tmp;
		} else {
			return;
		}

		bubbleDown(childIndex);
	}
}