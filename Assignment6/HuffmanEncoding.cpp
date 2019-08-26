/**********************************************************
 * File: HuffmanEncoding.cpp
 *
 * Implementation of the functions from HuffmanEncoding.h.
 * Most (if not all) of the code that you write for this
 * assignment will go into this file.
 */

#include "HuffmanEncoding.h"
#include "pqueue.h"
#include "foreach.h"

/* Function prototypes */

void transferToQueue(ext_char key, int value, PriorityQueue<Node*> & pq);
Node* mergeNodes(Node* firstNode, Node* secondNode);
void getCharMap(Node* encodingTree, Map<ext_char, string> & charMap);
void addCharsToMap(Node* encodingTree, Map<ext_char, string> & charMap, string prefix);
void writeAsBit(string bitStr, obstream & outfile);
ext_char findNextChar(ibstream& infile, Node* encodingTree);

/* Function: getFrequencyTable
 * Usage: Map<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * Given an input stream containing text, calculates the
 * frequencies of each character within that text and stores
 * the result as a Map from ext_chars to the number of times
 * that the character appears.
 *
 * This function will also set the frequency of the PSEUDO_EOF
 * character to be 1, which ensures that any future encoding
 * tree built from these frequencies will have an encoding for
 * the PSEUDO_EOF character.
 */
Map<ext_char, int> getFrequencyTable(istream& file) {
	// Create map first
	Map<ext_char, int> charMap;

	// Read file character by character
	char ch;
	while (file.get(ch)) {
		// If key is not found, it will create it and put 1 as its value. 
		// Otherwise, it will increase its value by 1.
		charMap.put(ch, charMap[ch] + 1);
	}

	// Add PSEUDO_EOF character
	charMap.put(PSEUDO_EOF, 1);

	return charMap;	
}

/* Function: buildEncodingTree
 * Usage: Node* tree = buildEncodingTree(frequency);
 * --------------------------------------------------------
 * Given a map from extended characters to frequencies,
 * constructs a Huffman encoding tree from those frequencies
 * and returns a pointer to the root.
 *
 * This function can assume that there is always at least one
 * entry in the map, since the PSEUDO_EOF character will always
 * be present.
 */
Node* buildEncodingTree(Map<ext_char, int>& frequencies) {
	// Populate priority queue
	PriorityQueue<Node*> pq;
	foreach (ext_char key in frequencies) {
		transferToQueue(key, frequencies[key], pq);
	}

	// Merge nodes until only one remains
	while (!(pq.size() == 1)) {
		Node *firstNode = pq.dequeue();
		Node *secondNode = pq.dequeue();
		Node *merged = mergeNodes(firstNode, secondNode);
		pq.enqueue(merged, merged->weight);
	}

	// Return the only node in priority queue
	return pq.dequeue();
}

/* This method merges two nodes and returns pointer to the merged node. */
Node* mergeNodes(Node* firstNode, Node* secondNode) {
	Node *merged = new Node;
	merged->character = NOT_A_CHAR;
	merged->zero = firstNode;
	merged->one = secondNode;
	merged->weight = firstNode->weight + secondNode->weight;
	return merged;
}

/* This function converts frequency key-value pair to node and adds it in priority queue. */
void transferToQueue(ext_char key, int value, PriorityQueue<Node*> & pq) {
	Node *node = new Node;
	node->character = key;
	node->zero = NULL;
	node->one = NULL;
	node->weight = value;
	pq.enqueue(node, node->weight);
}

/* Function: freeTree
 * Usage: freeTree(encodingTree);
 * --------------------------------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.
 */
void freeTree(Node* root) {
	while (root != NULL) {
		freeTree(root->zero);
		freeTree(root->one);
		// Delete root node and make root point to null
		Node *tmp = root;
		root = NULL;
		delete tmp;
	}
}

/* Function: encodeFile
 * Usage: encodeFile(source, encodingTree, output);
 * --------------------------------------------------------
 * Encodes the given file using the encoding specified by the
 * given encoding tree, then writes the result one bit at a
 * time to the specified output file.
 *
 * This function can assume the following:
 *
 *   - The encoding tree was constructed from the given file,
 *     so every character appears somewhere in the encoding
 *     tree.
 *
 *   - The output file already has the encoding table written
 *     to it, and the file cursor is at the end of the file.
 *     This means that you should just start writing the bits
 *     without seeking the file anywhere.
 */ 
void encodeFile(istream& infile, Node* encodingTree, obstream& outfile) {
	Map<ext_char, string> charMap;
	getCharMap(encodingTree, charMap);

	char ch;
	while(infile.get(ch)) {
		writeAsBit(charMap[ch], outfile);
	}
	writeAsBit(charMap[PSEUDO_EOF], outfile);
}

/* This function reads passed string recursively and inserts corresponding bits in obstream */
void writeAsBit(string bitStr, obstream & outfile) {
	if (bitStr != "") {
		if (bitStr[0] == '0') {
			outfile.writeBit(0);
		}
		if (bitStr[0] == '1') {
			outfile.writeBit(1);
		}
		writeAsBit(bitStr.substr(1), outfile);
	}
}

/* This wrapper function looks for a character in a tree adds it as a key in a map. 
 * Corresponding value is that char's byte value. */
void getCharMap(Node* encodingTree, Map<ext_char, string> & charMap) {
	addCharsToMap(encodingTree, charMap, "");
}

/* This is underlying recursive implementation of getCharMap function */
void addCharsToMap(Node* encodingTree, Map<ext_char, string> & charMap, string prefix) {
	if(encodingTree->character != NOT_A_CHAR) {
		charMap.put(encodingTree->character, prefix);
	} else {
		addCharsToMap(encodingTree->zero, charMap, prefix + '0');
		addCharsToMap(encodingTree->one, charMap, prefix + '1');
	}
}

/* Function: decodeFile
 * Usage: decodeFile(encodedFile, encodingTree, resultFile);
 * --------------------------------------------------------
 * Decodes a file that has previously been encoded using the
 * encodeFile function.  You can assume the following:
 *
 *   - The encoding table has already been read from the input
 *     file, and the encoding tree parameter was constructed from
 *     this encoding table.
 *
 *   - The output file is open and ready for writing.
 */
void decodeFile(ibstream& infile, Node* encodingTree, ostream& file) {
	// Doesn't work yet
	while(true) {
		ext_char nextChar = findNextChar(infile, encodingTree);
		if (nextChar == PSEUDO_EOF) break;
		file.put(nextChar);
	}
}

ext_char findNextChar(ibstream& infile, Node* encodingTree) {
	if (encodingTree->character != NOT_A_CHAR) {
		return encodingTree->character;
	} else {
		int dir = infile.readBit();
		if (dir == 0) {
			findNextChar(infile, encodingTree->zero);
		}
		if (dir == 1) {
			findNextChar(infile, encodingTree->one);
		}		
	}
}

/* Function: writeFileHeader
 * Usage: writeFileHeader(output, frequencies);
 * --------------------------------------------------------
 * Writes a table to the front of the specified output file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to decompress input files once they've been
 * compressed.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * readFileHeader function defined below this one so that it
 * can properly read the data back.
 */
void writeFileHeader(obstream& outfile, Map<ext_char, int>& frequencies) {
	/* The format we will use is the following:
	 *
	 * First number: Total number of characters whose frequency is being
	 *               encoded.
	 * An appropriate number of pairs of the form [char][frequency][space],
	 * encoding the number of occurrences.
	 *
	 * No information about PSEUDO_EOF is written, since the frequency is
	 * always 1.
	 */
	 
	/* Verify that we have PSEUDO_EOF somewhere in this mapping. */
	if (!frequencies.containsKey(PSEUDO_EOF)) {
		error("No PSEUDO_EOF defined.");
	}
	
	/* Write how many encodings we're going to have.  Note the space after
	 * this number to ensure that we can read it back correctly.
	 */
	outfile << frequencies.size() - 1 << ' ';
	
	/* Now, write the letter/frequency pairs. */
	foreach (ext_char ch in frequencies) {
		/* Skip PSEUDO_EOF if we see it. */
		if (ch == PSEUDO_EOF) continue;
		
		/* Write out the letter and its frequency. */
		outfile << char(ch) << frequencies[ch] << ' ';
	}
}

/* Function: readFileHeader
 * Usage: Map<ext_char, int> freq = writeFileHeader(input);
 * --------------------------------------------------------
 * Reads a table to the front of the specified input file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to reconstruct the encoding tree for that file.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * writeFileHeader function defined before this one so that it
 * can properly write the data.
 */
Map<ext_char, int> readFileHeader(ibstream& infile) {
	/* This function inverts the mapping we wrote out in the
	 * writeFileHeader function before.  If you make any
	 * changes to that function, be sure to change this one
	 * too!
	 */
	Map<ext_char, int> result;
	
	/* Read how many values we're going to read in. */
	int numValues;
	infile >> numValues;
	
	/* Skip trailing whitespace. */
	infile.get();
	
	/* Read those values in. */
	for (int i = 0; i < numValues; i++) {
		/* Get the character we're going to read. */
		ext_char ch = infile.get();
		
		/* Get the frequency. */
		int frequency;
		infile >> frequency;
		
		/* Skip the space character. */
		infile.get();
		
		/* Add this to the encoding table. */
		result[ch] = frequency;
	}
	
	/* Add in 1 for PSEUDO_EOF. */
	result[PSEUDO_EOF] = 1;
	return result;
}

/* Function: compress
 * Usage: compress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the file whose contents are specified by the input
 * ibstream, then writes the result to outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void compress(ibstream& infile, obstream& outfile) {
	Map<ext_char, int> frequencies = getFrequencyTable(infile);
	Node *tree = buildEncodingTree(frequencies);
	infile.rewind();
	writeFileHeader(outfile, frequencies);
	encodeFile(infile, tree, outfile);
	freeTree(tree);
}

/* Function: decompress
 * Usage: decompress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman decompressor.
 * Decompresses the file whose contents are specified by the
 * input ibstream, then writes the decompressed version of
 * the file to the stream specified by outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void decompress(ibstream& infile, ostream& outfile) {
	Map<ext_char, int> frequencies = readFileHeader(infile);
	Node *tree = buildEncodingTree(frequencies);
	decodeFile(infile, tree, outfile);
	freeTree(tree);
}

