/*
 * File: NumericConversions.cpp
 * ---------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Numeric Conversions problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include <string>
#include "console.h"
using namespace std;

/* Function prototypes */

string intToString(int n);
int stringToInt(string str);
string digitToCharString(int digit);
int charStringToDigit(string str);

/* Main program */

int main() {
	cout << intToString(-2173) << endl;
	cout << intToString(0) << endl;
	cout << intToString(76236) << endl;
	cout << stringToInt("-2173") << endl;
	cout << stringToInt("0") << endl;
	cout << stringToInt("-76236") << endl;
    return 0;
}

/* Converts integer to string, recursively 
 * I _assume_ that passed value of n is always valid
 */ 
string intToString(int n) {
	// Choose negative sign if n is less than 0
	string sign = "";
	if (n < 0) {
		n = -n;
		sign = "-";
	}

	int n1 = n / 10;
	int n2 = n % 10;

	// Check for base case
	if (n1 == 0) {
		return digitToCharString(n);
	}

	return sign + intToString(n1) + intToString(n2);
}

/* Converts string to integer, recursively
 * I _assume_ that passed value of n is always valid
 */
int stringToInt(string str) {
	// Choose negative sign if first char is "-"
	int sign = 1;
	if (str.at(0) == '-') {
		str = str.substr(1);
		sign *= -1;
	}

	// Check for base case
	if (str.size() == 1) {
		return charStringToDigit(str);
	}
	
	string str1 = str.substr(0, str.size() - 1);
	string str2 = string() + str[str.size() - 1];
	return sign * (10 * stringToInt(str1) + stringToInt(str2));
}

string digitToCharString(int digit) {
	return string() + char(digit + '0');
}

int charStringToDigit(string str) {
	return str.at(0) - '0';
}