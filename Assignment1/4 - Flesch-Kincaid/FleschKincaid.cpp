/*
 * File: FleschKincaid.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Flesch-Kincaid problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
#include <fstream>
#include "tokenscanner.h"
#include <string>
#include <cctype>
using namespace std;

// Define constants
const double C0 = -15.59;
const double C1 = 0.39;
const double C2 = 11.8;

const string SENTENCE_FINISHERS = ".?!";
const string VOWELS = "aeiouyAEIOUY";

// Define global variables
int words, sentences, syllables = 0;

double computeGrade() {
	return C0 + C1 * words / sentences + C2 * syllables / words;
}

int countSyllables(string str) {
	int res = 0;
	bool lastWasVowel = false;

	for (int i = 0; i < str.size(); i++) {
		// If last letter is e/E, don't count it as a syllable
		if (i == str.size() - 1 && (str[i] == 'e' || str[i] == 'E')) {
			break;
		}

		if (VOWELS.find(str[i]) != string::npos) {
			if (!lastWasVowel) {
				res++;		
			}
			lastWasVowel = true;
		} else {
			lastWasVowel = false;
		}
	}

	// Address zero syllable case
	if (res == 0) {
		res++;
	}

	return res;
}

void categorizeToken(string str) {
	// Count up words, sentences and syllables
	if (isalpha(str[0])) {
		words++;
		syllables += countSyllables(str);
	} else if (SENTENCE_FINISHERS.find(str) != string::npos){
		sentences++;
	}
}

void computeFleschKincaid(ifstream &input){
	TokenScanner scanner(input);
	scanner.ignoreWhitespace();
	// Add character which should be considered as a part of word (for ex. "isn't" is a single word)
	scanner.addWordCharacters("'");
	
	while (scanner.hasMoreTokens()) {
		// Decide what each token means
		categorizeToken(scanner.nextToken());
	}

	// Avoid division by zero error
	if (sentences == 0) {
		sentences++;
	}
	
	if (words == 0) {
		words++;
	}
	
	// Print out answers
	cout << "Words: " << words << endl;
	cout << "Syllables: " << syllables << endl;
	cout << "Sentences: " << sentences << endl;
	cout << "Grade level: " << computeGrade() << endl;
}

/* This function waits for valid user input and calls computeFleschKincaid function */
void promtUser() {
	string fileName;
	fileName = getLine("Enter file name: ");

	ifstream ifs;
	ifs.open(fileName.c_str());

	if (ifs) {
		computeFleschKincaid(ifs);
	} else {
		cout << "Unable to open that file. Try again." << endl;
		promtUser();
	}
}

int main() {
    promtUser();
    return 0;
}