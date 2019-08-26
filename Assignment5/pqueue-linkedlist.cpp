/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	len = 0;
	head = NULL;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	while (head != NULL) {
		Cell *next = head->next;
		delete head;
		head = next;
	}
}

int LinkedListPriorityQueue::size() {
	return len;
}

bool LinkedListPriorityQueue::isEmpty() {
	return len == 0;
}

void LinkedListPriorityQueue::enqueue(string value) {
	if (isEmpty()) {
		Cell *c = createCell(value, NULL);
		head = c;
		len++;
		return;
	}
	len++;
	addToLinkedList(value, head);
}

string LinkedListPriorityQueue::peek() {
	if (head == NULL) error("Attempting to peek in an empty queue");
	return head->value;
}

string LinkedListPriorityQueue::dequeueMin() {
	if (head == NULL) error("Attempting to dequeue an element from an empty queue");
	len--;
	string value = head->value;
	Cell *tmp = head;
	head = head->next;
	delete tmp;
	return value;
}

Cell *LinkedListPriorityQueue::createCell(string value, Cell *next) {
	Cell *cell = new Cell;
	cell->value = value;
	cell->next = next;
	return cell;
}

void LinkedListPriorityQueue::addToLinkedList(string value, Cell* & head) {
	Cell *prev = NULL;
	for (Cell *c = head; c != NULL; c = c->next) {
		if (c->value > value) {
			if (prev == NULL) {
				head = createCell(value, c);
				return;
			}
			prev->next = createCell(value, c);
			return;
		}
		prev = c;
	}
	prev->next = createCell(value, NULL);
}