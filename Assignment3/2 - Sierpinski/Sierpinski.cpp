/*
 * File: Sierpinski.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Sierpinski problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gwindow.h"
#include <math.h>
using namespace std;

void drawSierpinski(int edge, int order, double x, double y, GWindow & gw);
void drawTriangle(int edge, double x, double y, GWindow & gw);

// Define constants
const int EDGE = 300;
const int ORDER = 5;

int main() {
	GWindow gw;
	drawSierpinski(EDGE, ORDER, gw.getWidth() / 2 - EDGE / 2, gw.getHeight() / 2 + sqrt(3.0) * EDGE / 4, gw);
	return 0;
}

void drawSierpinski(int edge, int order, double x, double y, GWindow & gw) {
	if (order == 0) {
		// Base case
		drawTriangle(edge, x, y, gw);
	} else {
		// Inductive step
		drawSierpinski(edge / 2, order - 1, x, y, gw);
		drawSierpinski(edge / 2, order - 1, x + edge / 2, y, gw);
		drawSierpinski(edge / 2, order - 1, x + edge / 4, y - sqrt(3.0) * edge / 4, gw);
	}
}

/* Draws triangle with its bottom left corner located at (x, y) */
void drawTriangle(int edge, double x, double y, GWindow & gw) {
	GPoint startPoint(x, y);
	for (int i = 0; i < 3; i++) {
		startPoint = gw.drawPolarLine(startPoint, edge, 60 - 120 * i);
	}
}