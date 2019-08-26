/*
 * File: UniversalHealthCoverage.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the UniversalHealthCoverage problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */
#include <iostream>
#include <string>
#include "set.h"
#include "vector.h"
#include "console.h"
using namespace std;

/* Function: canOfferUniversalCoverage(Set<string>& cities,
 *                                     Vector< Set<string> >& locations,
 *                                     int numHospitals,
 *                                     Vector< Set<string> >& result);
 * Usage: if (canOfferUniversalCoverage(cities, locations, 4, result)
 * ==================================================================
 * Given a set of cities, a list of what cities various hospitals can
 * cover, and a number of hospitals, returns whether or not it's
 * possible to provide coverage to all cities with the given number of
 * hospitals.  If so, one specific way to do this is handed back in the
 * result parameter.
 */
bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
                               Vector< Set<string> >& result);


int main() {
	// Built test case
    Set<string> cities;
	cities.add("A");
	cities.add("B");
	cities.add("C");
	cities.add("D");
	cities.add("E");
	cities.add("F");

	Set<string> loc1;
	loc1.add("A");
	loc1.add("B");
	loc1.add("C");

	Set<string> loc2;
	loc2.add("A");
	loc2.add("C");
	loc2.add("D");

	Set<string> loc3;
	loc3.add("B");
	loc3.add("F");

	Set<string> loc4;
	loc4.add("C");
	loc4.add("E");
	loc4.add("F");

	Vector< Set<string> > locations;
	locations.add(loc1);
	locations.add(loc2);
	locations.add(loc3);
	locations.add(loc4);

	Vector< Set<string> > result;
	
	// Run the actual code
	if(canOfferUniversalCoverage(cities, locations, 3, result)) {
		cout << result << endl;
	} else {
		cout << "Cannot offer universal coverage" << endl;
	}
    return 0;
}

bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
							   Vector< Set<string> >& result) {
	// If the answer exists, there are two possible scenarios:
	// 1. the first hospital in the set is in the resulting set
	// 2. It isn't
	// We'll follow both scenarios. If neither succeeds, then the answer doesn't exist

    if (cities.size() > 0 && (numHospitals == 0 || locations.size() == 0)) {
		return false;
    }
	if (cities.size() == 0 && numHospitals >= 0 && locations.size() >= 0) {
		return true;
	}
	// For the first scenario, we emit first location and corresponding cities, 
	// also decrease numHospitals and add location in results
	//
	// For the second scenario, we don't change result, but remove first location and since
	// we don't use it, numHospital and cities remain the same
	Set<string> firstLoc;
	firstLoc = locations.get(0);
	
	Vector< Set<string> > newLocs;
	newLocs = locations;
	newLocs.remove(0);

	if (canOfferUniversalCoverage(cities - firstLoc, newLocs, numHospitals - 1, result)) {
		result.add(firstLoc);
		return true;
	}
	if (canOfferUniversalCoverage(cities, newLocs, numHospitals, result)) {
		return true;
	}
	return false;
}