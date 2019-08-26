/*
 * File: InverseGenetics.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Inverse Genetics problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include <fstream>
#include "set.h"
#include "map.h"
#include "console.h"
#include "foreach.h"
using namespace std;

/* Function: listAllRNAStrandsFor(string protein,
 *                                Map<char, Set<string> >& codons);
 * Usage: listAllRNAStrandsFor("PARTY", codons);
 * ==================================================================
 * Given a protein and a map from amino acid codes to the codons for
 * that code, lists all possible RNA strands that could generate
 * that protein
 */
void listAllRNAStrandsFor(string protein, Map<char, Set<string> > & codons);

/* Function: loadCodonMap();
 * Usage: Map<char, Lexicon> codonMap = loadCodonMap();
 * ==================================================================
 * Loads the codon mapping table from a file.
 */
Map<char, Set<string> > loadCodonMap();

Set<string> getStrandSetFor(string protein, Map<char, Set<string> > & codons);

int main() {
    /* Load the codon map. */
    Map<char, Set<string> > codons = loadCodonMap();

    listAllRNAStrandsFor("KWS", codons);
    return 0;
}

void listAllRNAStrandsFor(string protein, Map<char, Set<string> > & codons) {
	foreach (string rnaStrand in getStrandSetFor(protein, codons)) {
		cout << rnaStrand << endl;
	}
}

Set<string> getStrandSetFor(string protein, Map<char, Set<string> > & codons) {
	if (protein.length() == 1) {
		return codons.get(protein[0]);
	} else {
		Set<string> result;
		foreach (string sequencePrefix in getStrandSetFor(protein.substr(0, 1), codons)) {
			foreach (string shorterSequence in getStrandSetFor(protein.substr(1), codons)) {
				result.add(sequencePrefix + shorterSequence);
			}
		}
		return result;
	}
}

/* You do not need to change this function. */
Map<char, Set<string> > loadCodonMap() {
    ifstream input("codons.txt");
    Map<char, Set<string> > result;

    /* The current codon / protein combination. */
    string codon;
    char protein;

    /* Continuously pull data from the file until all data has been
     * read.
     */
    while (input >> codon >> protein) {
        result[protein] += codon;
    }

    return result;
}
