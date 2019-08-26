/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "foreach.h"
#include "point.h"
#include "gobjects.h"
#include "gevents.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
void play(GWindow gw);
void arrayToVector(const string arr[], Vector<string> & vec, int boardLen);
void shuffle(Vector<string> & vec);
void fillBoardAndGrid(Vector<string> & vec, Grid<char> & charGrid);
char randomChar(string str);
bool forceBoardConfig();
void applyForcedConfig(Vector<string> & vec, int boardLen);
bool foundWordInGrid(string input, Grid<char> & charGrid, Grid<bool> & boolGrid);
bool isValidWord(string word, Lexicon & lexicon, Lexicon & humanWords);
bool existsWordFromStartingPoint(string word, Grid<char> & charGrid, Grid<bool> & boolGrid, Point startPoint);
Vector<Point> getValidMoveSet(Point startPoint, Grid<bool> & boolGrid);
void highlight(Grid<bool> & boolGrid);
bool clearGrid(Grid<bool> & grid);
void findAllWords(Lexicon & englishWords, Lexicon & forbiddenWords, Grid<char> & charGrid, Grid<bool> & boolGrid);
Lexicon findAllWordsFromStartingPositionWithPrefix(Point pt, string prefix, Lexicon & englishWords, Grid<char> & charGrid, Grid<bool> & boolGrid);
bool bigCubes();
string getGraphicsInput(GWindow gw);

/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    giveInstructions();
	play();

    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

void play(GWindow gw) {
	// Ask for board size
	int boardLen = bigCubes() ? 5 : 4;

	// Draw board, initialize vector, grids and lexicons
	drawBoard(boardLen, boardLen);
	Vector<string> cubes;
	Grid<char> charGrid(boardLen, boardLen);
	Grid<bool> boolGrid(boardLen, boardLen);
	Lexicon englishWords("EnglishWords.dat");
	Lexicon humanWords;	
	
	// Fill board with cubes
	if(forceBoardConfig()) {
		applyForcedConfig(cubes, boardLen);
	} else {
		arrayToVector(BIG_BOGGLE_CUBES, cubes, boardLen);
		shuffle(cubes);	
	}
	fillBoardAndGrid(cubes, charGrid);

	// Let human enter words
	while (true) {
		string input;
		input = toUpperCase(getLine("\nEnter word: "));
		if (input == "") break;
		if (isValidWord(input, englishWords, humanWords) && foundWordInGrid(input, charGrid, boolGrid)) {
			humanWords.add(input);
			recordWordForPlayer(input, HUMAN);
			highlight(boolGrid);
			pause(1000);
			// boolGrid is now clear, so we call this function again to remove highlights from board
			highlight(boolGrid);
		} else {
			cout << "That's not a valid input!" << endl;
		}
	}

	findAllWords(englishWords, humanWords, charGrid, boolGrid);

	while(true) {
		string input = getLine("\nPlay again? yes/no: ");
		if (input == "no") break;
		if (input == "yes") play();
	}
}

void arrayToVector(const string arr[], Vector<string> & vec, int boardLen) {
	for (int i = 0; i < boardLen * boardLen; i++) {
		vec.add(arr[i]);
	}
}

void shuffle(Vector<string> & vec) {
	for (int i = 0; i < vec.size(); i++) {
		int rand = randomInteger(0, vec.size() - 1);
		string temp = vec[i];
		vec[i] = vec[rand];
		vec[rand] = temp;
	}
}

/* Fills board using vector while conveniently saving it in grid */
void fillBoardAndGrid(Vector<string> & vec, Grid<char> & charGrid) {
	for (int i = 0; i < charGrid.nRows; i++) {
		for (int j = 0; j < charGrid.nCols; j++) {
			char letter = randomChar(vec[i * charGrid.nCols + j]);
			labelCube(i, j, letter);
			charGrid.set(i, j, letter);
		}
	}
}

char randomChar(string str) {
	return str[randomInteger(0, str.length() - 1)];
}

/* Returns true if user wants to force board configuration */
bool forceBoardConfig() {
	while(true) {
		string answer = getLine("\nForce board config? yes/no: ");
		if (answer == "yes") return true;
		if (answer == "no") return false;
		cout << "Enter valid answer, please." << endl;
	}
}

/* Lets user to enter custom configuration and applies it */
void applyForcedConfig(Vector<string> & vec, int boardLen) {
	while (true) {
		string forcedConfig = getLine("\nEnter custom configuration: ");
		if (forcedConfig.size() >= boardLen * boardLen) {
			for (int i = 0; i < boardLen * boardLen; i++) {
				vec.add(toUpperCase(forcedConfig[i] + string()));
			}
			break;
		} else {
			cout << "Enter valid configuration, please" << endl;
		}
	}
}

/* Seeks specific word on gameboard (It's a wrapper function) */
bool foundWordInGrid(string word, Grid<char> & charGrid, Grid<bool> & boolGrid) {
	for (int i = 0; i < charGrid.nRows; i++) {
		for (int j = 0; j < charGrid.nCols; j++) {
			Point pt(i, j);
			if (existsWordFromStartingPoint(word, charGrid, boolGrid, pt)) {
				return true;
			}
		}
	}
	return false;
}

/* Returns true if word is valid according to game rules */
bool isValidWord(string word, Lexicon & lexicon, Lexicon & humanWords) {
	return (word.size() >= 4 && lexicon.contains(word) && !humanWords.contains(word));
}

/* Recursive function which checks if word exists from specified starting point */
bool existsWordFromStartingPoint(string word, Grid<char> & charGrid, Grid<bool> & boolGrid, Point startPoint) {
	if (word == "") return true;
	boolGrid.set(startPoint.getX(), startPoint.getY(), true);
	Vector<Point> moves = getValidMoveSet(startPoint, boolGrid);
	if (word[0] == charGrid.get(startPoint.getX(), startPoint.getY())) {
		if (moves.isEmpty() && word.size() == 1) {
			return true;
		}
		foreach (Point pt in moves) {
			if (existsWordFromStartingPoint(word.substr(1), charGrid, boolGrid, pt)) {
				return true;
			}
		}
	}
	boolGrid.set(startPoint.getX(), startPoint.getY(), false);
	return false;
}

/* Returns a vector containing all valid points to move to */
Vector<Point> getValidMoveSet(Point startPoint, Grid<bool> & boolGrid) {
	Vector<Point> validMoves;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = startPoint.getX() + i;
			int y = startPoint.getY() + j;
			/* 
			There are 2 criteria:
			1. Move should be in bounds of a grid
			2. Grid value should not be true at that point
			*/
			if (boolGrid.inBounds(x, y) && !boolGrid.get(x, y)) {
				validMoves.add(Point(x,y));
			}
		}
	}
	return validMoves;
}

/* Highlights the last guessed word on board */
void highlight(Grid<bool> & boolGrid) {
	for (int i = 0; i < boolGrid.nRows; i++) {
		for (int j = 0; j < boolGrid.nCols; j++) {
			highlightCube(i, j, boolGrid.get(i, j));
			// Clear grid at the same time
			boolGrid.set(i, j, false);
		}
	}
}

/* Computer finds every word on board using this funciton */
void findAllWords(Lexicon & englishWords, Lexicon & forbiddenWords, Grid<char> & charGrid, Grid<bool> & boolGrid) {
	Lexicon allWords;
	for (int i = 0; i < charGrid.nRows; i++) {
		for (int j = 0; j < charGrid.nCols; j++) {
			Point pt(i, j);
			foreach(string newWord in findAllWordsFromStartingPositionWithPrefix(pt, "", englishWords, charGrid, boolGrid)) {
				allWords.add(newWord);	
			}
		}
	}
	foreach (string s in allWords) {
		if (isValidWord(s, englishWords, forbiddenWords)){
			recordWordForPlayer(s, COMPUTER);
		}
	}
}

Lexicon findAllWordsFromStartingPositionWithPrefix(Point pt, string prefix, Lexicon & englishWords, Grid<char> & charGrid, Grid<bool> & boolGrid) {
	Lexicon words;
	string currentStr = prefix + charGrid.get(pt.getX(), pt.getY());
	boolGrid.set(pt.getX(), pt.getY(), true);

	if (englishWords.containsPrefix(currentStr)) {
		if (englishWords.contains(currentStr)) {
			words.add(currentStr);
		}
		Vector<Point> moves = getValidMoveSet(pt, boolGrid);
		foreach(Point p in moves) {
			foreach(string foundWord in findAllWordsFromStartingPositionWithPrefix(p, currentStr, englishWords, charGrid, boolGrid)) {
				words.add(foundWord);
			}
		}
	}
	boolGrid.set(pt.getX(), pt.getY(), false);
	return words;
}

bool bigCubes() {
	while (true) {
		string input = getLine("\nDo you want a big (5x5) board instead of a normal (4x4) one? yes/no: ");
		if (input == "yes") return true;
		if (input == "no") return false;
		cout << "Enter valid answer, please" << endl;
	}
}