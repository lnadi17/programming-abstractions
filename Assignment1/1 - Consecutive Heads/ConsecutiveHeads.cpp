/*
 * File: ConsecutiveHeads.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Consecutive Heads problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "console.h"
#include "random.h"
using namespace std;

// Constants
const double PROBABILITY = 0.1;

// Function prototype
void flipCoin(int p);

int main() {
	// Define counters
	int heads = 0;
	int steps = 0;

	while (true) {
		// Toss coin
		if (randomChance(PROBABILITY)) {
			heads++;
			cout << "HEADS" << endl;
		} else {
			heads = 0;
			cout << "TAILS" << endl;
		}
		steps++;
		
		// If three consecutive heads, stop
		if (heads == 3) {
			break;
		}
	}

	cout << "It took " << steps << " steps to get 3 consecutive heads." << endl;

    return 0;
}
