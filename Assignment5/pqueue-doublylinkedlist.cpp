/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	len = 0;
	head = NULL;
	tail = NULL;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	// TODO: Fill this in!
}

int DoublyLinkedListPriorityQueue::size() {
	return len;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
	return len == 0;
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	len++;
	if (head == NULL) {
		head = createCell(NULL, value, NULL);
		//tail = head;
		return;
	}
	addToHead(value);
}

string DoublyLinkedListPriorityQueue::peek() {
	if (head == NULL) error("Queue is empty!");
	return getMinCellPointer()->value;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	if (head == NULL) error("Queue is empty");
	len--;
	string value = getMinCellPointer()->value;

	dCell *tmp = getMinCellPointer();
	dCell *minPrev = getMinCellPointer()->prev;
	dCell *minNext = getMinCellPointer()->next;

	if (minPrev == NULL && minNext == NULL) {
		head == NULL;
		delete tmp;
		return value;
	}

	if (minPrev != NULL) {
		minPrev->next = minNext;
	} else {
		head = minNext;
		minNext->prev = NULL;
	}

	if (minNext != NULL) {
		minNext->prev = minPrev;
	} else {
		minPrev->next = NULL;
	}

	delete tmp;
	return value;
}

dCell *DoublyLinkedListPriorityQueue::createCell(dCell *prev, string value, dCell *next){
	dCell *c = new dCell;
	c->prev = prev;
	c->value = value;
	c->next = next;
	return c;
}

void DoublyLinkedListPriorityQueue::addToHead(string value) {
	dCell *oldHead = head;
	head = createCell(NULL, value, oldHead);
}

dCell *DoublyLinkedListPriorityQueue::getMinCellPointer() {
	dCell *minCell = head;
	for(dCell *c = head; c != NULL; c = c->next) {
		if (c->value < minCell->value) {
			minCell = c;
		}
	}
	return minCell;
}

