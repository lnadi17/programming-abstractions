/*
 * File: Combinations.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Combinations problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "gwindow.h"
#include "gobjects.h"
#include "strlib.h"
using namespace std;

// Constants
const int WIDTH = 400;
const int HEIGHT = 400;

const int TRIANGLE_HEIGHT = 10;

// Function prototypes
int combinations(int n, int k);
void drawTriangle(int height, GWindow &window);
void drawLabel(int number, int i, int j, GWindow &window);

int main() {
	GWindow window(WIDTH, HEIGHT);
	drawTriangle(TRIANGLE_HEIGHT, window);
    return 0;
}

/* This function computes number of combinations for arguments N, K
 * I _assume_ N is always more than or equal to K
 */
int combinations(int n, int k) {
	if (n == k || k == 0) {
		return 1;
	} else {
		return combinations(n - 1, k) + combinations (n - 1, k - 1);
	}
}

/* This function draws pascal triangle given triangle height (number of rows) and GWindow */
void drawTriangle(int height, GWindow &window) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j <= i; j++) {
			int currentCombination = combinations(i, j);
			drawLabel(currentCombination, i, j, window);
		}
	}
}

/* This function draws label on GWindow using number and it's position in pascal's triangle */
void drawLabel(int number, int i, int j, GWindow &window) {
	GLabel label(integerToString(number), 30, 30);
	window.add(&label, WIDTH / 2 - (25 * i / 2) + 25 * j, HEIGHT / 4 + 20 * i);
}
