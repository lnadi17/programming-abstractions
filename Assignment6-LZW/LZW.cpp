#include "LZW.h"
#include "console.h"
#include "simpio.h"
#include <iostream>
#include <cmath>
using namespace std;

const int LAST_ASCII_VALUE = 255;

/* Function prototypes */
void compress(ibstream& infile, obstream& outfile);
void decompress(ibstream& infile, ostream& outfile);
Map<string, int> createEncodingMap(ibstream& infile);
void writeBits(obstream& outfile, int code);
string toBinary(int n);
bool readBitBlock(ibstream & infile, int & bits);
Map<int, string> createDecodingMap(ibstream & infile);
void writeStringToStream(string str, ostream & outfile);

/* This implementation uses 9 bits for encoding */
int main() {
	string input = getLine("Enter file name to encode: ");
	string output = getLine("Enter file name to output encoded file: ");

	// Open files
	ifbstream infile;
	infile.open(input.c_str());
	ofbstream outfile;
	outfile.open(output.c_str());

	// Compress file
	compress(infile, outfile);

	// Rewind and close files
	infile.close();
	outfile.close();
	
	input = getLine("Enter file name to decode: ");
	output = getLine("Enter file name to output decoded file: ");

	// Open files again
	infile.open(input.c_str());
	outfile.open(output.c_str());

	// Decompress file
	decompress(infile, outfile);

	// Close files again
	infile.close();
	outfile.close();

	return 0;
}

void compress(ibstream& infile, obstream& outfile) {
	Map<string, int> map = createEncodingMap(infile);
	infile.rewind();
	int code = LAST_ASCII_VALUE + 1;
	
	char ch;
	while (infile.get(ch)) {
		// Find the longest string that matches current input 
		string current = string() + ch;
		while (map.containsKey(current)) {
			char next;
			if(!infile.get(next)) {current += " "; break;}
			current += next;
		}

		// Emit the dictionary index for the longest matching string to output
		writeBits(outfile, map[current.substr(0, current.size() - 1)]);

		// Step one character back
		infile.unget();

		// Add new string pattern to map only if we can represent it with 9 bits
		if (code <= 511) {
			map.put(current, code++);
		}
	}
}

// Writes integer in passed stream as a 9-bit block. Maximum value of code is 511 */
void writeBits(obstream & outfile, int code) {
	string bin = toBinary(code);

	// Add remaining zeroes to fill 9-bit block
	while ((9 - bin.size()) > 0) {
		bin = "0" + bin;
	}

	// Write corresponding bits in output stream
	while (bin.size() != 0) {
		outfile.writeBit(bin[0] == '0' ? 0 : 1);
		bin = bin.substr(1);
	}
}

/* Recursive decimal to binary convertion. 
 * Code taken from: https://stackoverflow.com/questions/22746429/c-decimal-to-binary-converting/22746526 */
string toBinary(int n) {
	string answer;
	if (n % 2 == 0) {
		answer = "0" + answer;
	} else {
		answer = "1" + answer;
	}

	if (n / 2 != 0) {
		answer = toBinary(n / 2) + answer;
	}

	return answer;
}	

void decompress(ibstream& infile, ostream& outfile) {
	int code = LAST_ASCII_VALUE + 1;
	infile.rewind();

	Map<int, string> map = createDecodingMap(infile);
	infile.rewind();
	
	int current;
	readBitBlock(infile, current);
	int next;
	while (readBitBlock(infile, next)) {
		// Write current block as separate characters to file
		writeStringToStream(map.get(current), outfile);
		
		// Concatenate current string and the first character of the next string
		char c = map.containsKey(next) ? map.get(next)[0] : map.get(current)[0];
		string toSave = map.get(current) + c;
		map.put(code++, toSave);

		// For next iteration, present next becomes future current
		current = next;
	}
	// We finish decompressing by writing one last character
	writeStringToStream(map.get(current), outfile);
}

void writeStringToStream(string str, ostream & outfile) {
	for (int i = 0; i < str.length(); i++) {
		outfile.put(str[i]);
	}
}
  
Map<int, string> createDecodingMap(ibstream & infile) {
	Map<int, string> map;
	int block;
	while (readBitBlock(infile, block)) {
		// If block corresponds to ordinary ASCII character, add it in map
		if (block <= LAST_ASCII_VALUE) {
			char ch = block;
			string val = string() + ch;
			map.put(block, val);
		}
	}
	return map;
}

/* Returns false if stream is exhausted */
bool readBitBlock(ibstream & infile, int & bits) {
	bool returnVal;
	bits = 0;
	for (int i = 8; i >= 0; i--) {
		int bit = infile.readBit();
		if (bit == 1) {
			bits += pow(2.0, i);
			returnVal = true;
		}
		if (bit == -1) {
			returnVal = false;
		}
	}
	return returnVal;
}

/* Returns map which maps strings of length one to their ASCII codes.
 * It's assumed that cursor is at the start of the stream. */
Map<string, int> createEncodingMap(ibstream & infile) {
	Map<string, int> map;

	char ch;
	while (infile.get(ch)) {
		map.put(string() + ch, ch);
	}

	return map;
}