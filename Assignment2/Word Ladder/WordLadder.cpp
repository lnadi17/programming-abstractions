/*
 * File: WordLadder.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Word Ladder problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include <string>
#include "strlib.h"
#include "console.h"
#include "lexicon.h"
#include "vector.h"
#include "queue.h"
#include "simpio.h"
using namespace std;

void createLadder(string & startWord, string & endWord, Lexicon & validWords, Vector<string> & finalLadder);
Vector<string> writeQueueToVector(Queue<string> & q, Vector<string> & v);
Lexicon getValidDiffs(string & word, Lexicon & lexicon);

int main() {
	string line = getLine("Enter data file name: ");
	// I assume that entered file name is valid
	Lexicon validWords(line);
	// Read input while converting them to the lower case at the same time (to avoid unnecessary errors)
	string startWord = toLowerCase(getLine("Enter start word: "));
	string endWord = toLowerCase(getLine("Enter end word: "));

	Vector<string> ladder;
	createLadder(startWord, endWord, validWords, ladder);

	if (ladder.size() != 0) {
		cout << ladder << endl;
	} else {
		cout << "Such ladder does not exist" << endl;
	}
	return 0;
}

/* Writes word ladder sequence in the last argument */
void createLadder(string & startWord, string & endWord, Lexicon & validWords, Vector<string> & finalLadder) {
	// Define lexicon of already used words
	Lexicon usedWords;
	// Initialize a queue with a ladder which has the value of startWord only
	Queue< Vector<string> > ladderQueue;
	Vector<string> currentLadder;
	currentLadder.add(startWord);
	ladderQueue.enqueue(currentLadder);

	while (!ladderQueue.isEmpty()) {
		currentLadder = ladderQueue.dequeue();
		if (endWord == currentLadder[currentLadder.size() - 1]) {
			finalLadder = currentLadder;
			// Return if the answer is found and is stored in desired vector
			return;
		}
		foreach (string validDiff in getValidDiffs(currentLadder[currentLadder.size() - 1], validWords)) {
			if (!usedWords.contains(validDiff)) {
				// Avoid copying vectors
				currentLadder.add(validDiff);
				ladderQueue.enqueue(currentLadder);
				currentLadder.remove(currentLadder.size() - 1);
				usedWords.add(validDiff);
			}
		}	
	}
	// This is reached when no answer is found
	return;
}

/* Returns all valid words which differ from first argument with only one letter */
Lexicon getValidDiffs(string & word, Lexicon & lexicon) {
	Lexicon result;
	string temp;
	for (int i = 0; i < word.length(); i++) {
		for (int j = 'a'; j < 'z'; j++) {
			 temp = word;
			 temp[i] = j;
			 if (lexicon.contains(temp)) {
				result.add(temp);
			 }
		}
	}
	return result;
}