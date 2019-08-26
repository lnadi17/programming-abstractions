/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Random Writer problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include <string>
#include <fstream>
#include "console.h"
#include "simpio.h"
#include "map.h"
#include "vector.h"
#include "foreach.h"
#include "random.h"
using namespace std;

void countFrequencies(string & source, int & markovNumber, Map< string, Vector<char> > & seedFrequencies);
void generateRandomSequence(string & destinationFile, int & markovNumber, Map< string, Vector<char> > & seedFrequencies);
string getMostFrequentSeed(Map< string, Vector<char> > & seedFrequencies);
char randomElement(Vector<char> & vect);

const int OUTPUT_LENGTH = 10000;


int main() {
	// I assume source and destination files are valid
	string sourceText = getLine("Enter source file name: ");
	string destinationText = getLine("Enter destination file name: ");
	// Get valid markov number
	int markovNumber;
	while (true) {
		markovNumber = getInteger("Enter Markov number (from 1 to 10): ");
		if (markovNumber >= 1 && markovNumber <= 10) {
			break;
		}
		cout << "Incorrect range. Try again" << endl;
	}

	Map< string, Vector<char> > seedFrequencies;
	countFrequencies(sourceText, markovNumber, seedFrequencies);
	generateRandomSequence(destinationText, markovNumber, seedFrequencies);
    return 0;
}

void countFrequencies(string & source, int & markovNumber, Map< string, Vector<char> > & seedFrequencies) {
	// Open source file
	ifstream ifs(source.c_str());
	// Iterate through all possible values and store all upcoming characters
	string keyAndVal;
	char ch;
	while (ifs.get(ch)) {
		keyAndVal += ch;
		if (keyAndVal.length() % (markovNumber + 1) == 0) {
			// If sufficient characters are read, split key/value pairs and add them to the map
			string key = keyAndVal.substr(0, markovNumber);
			char val = keyAndVal[markovNumber];
			// Get associated vector
			Vector<char> vect = seedFrequencies.get(key);
			// Append vector
			vect.add(val);
			// Put vector back in map
			seedFrequencies.put(key, vect);

			// Remove the first character of key/value pair and get ready for next iteration 
			// (which adds the next character and goes through this same process again)
			keyAndVal = keyAndVal.substr(1, keyAndVal.length() - 1);
		}
	}
	// Close source file
	ifs.close();
}

/* Outputs a random sequence based on seedFrequencies map */
void generateRandomSequence(string & destinationFile, int & markovNumber, Map< string, Vector<char> > & seedFrequencies) {
	ofstream ofs(destinationFile.c_str());
	string firstSeed = getMostFrequentSeed(seedFrequencies);
	ofs << firstSeed;
	string currentSeed = firstSeed;
	// Initializing index as markovNumber, because that many characters are already added
	for (int i = markovNumber; i < OUTPUT_LENGTH; i++) {
		char randChar;
		randChar = randomElement(seedFrequencies.get(currentSeed));
		ofs.put(randChar);
		currentSeed = currentSeed.substr(1) + randChar;
	}
	// Close output file
	ofs.close();
}

/* Returns the key which has the longest vector size as a value */
string getMostFrequentSeed(Map< string, Vector<char> > & seedFrequencies) {
	string mostFrequent;
	int currentMax = 0;
	foreach (string key in seedFrequencies) {
		if (seedFrequencies.get(key).size() > currentMax) {
			currentMax = seedFrequencies.get(key).size();
			mostFrequent = key;
		}
	}
	return mostFrequent;
}

// Returns random element from a vector
char randomElement(Vector<char> & vect) {
	if (vect.size() == 0) {
		return 0;
	}
	return vect.get(randomInteger(0, vect.size() - 1));
}