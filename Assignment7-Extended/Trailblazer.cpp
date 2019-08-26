/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "map.h"
#include "foreach.h"
#include "pqueue.h"
#include "random.h"
#include "vector.h"
using namespace std;

Set<Loc> getNeighbourSet(Loc loc, int xdim, int ydim);

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
Vector<Loc> shortestPath(Loc start, Loc end, Grid<double>& world,
						 double costFn(Loc from, Loc to, Grid<double>& world), 
						 double heuristic(Loc start, Loc end, Grid<double> & world)) {
	// Map locations to colors to easily check color when needed
	Map<Loc, Color> colors;
	// Map locations to locations to easily backtrack the path after
	Map<Loc, Loc> parents;
	// Create grid which stores travel cost from a starting location for each node
	Grid<double> costs(world.numRows(), world.numCols());
	// Creade grid which stores heuristics for each location
	Grid<double> heuristics(world.numRows(), world.numCols());
	// Create PQueue which stores lowest cost nodes first
	TrailblazerPQueue<Loc> pq;

	// Color all nodes gray, make costs equal to infinity and estimate costs, all at the same time
	for (int i = 0; i < world.numCols(); i++) {
		for (int j = 0; j < world.numRows(); j++) {
			// Put gray node in map
			Loc loc;
			loc.col = i;
			loc.row = j;
			colors.put(loc, GRAY);
			// Init costs as infinity
			costs.set(i, j, INT_MAX);
			// Init heuristics grid
			heuristics.set(j, i, heuristic(loc, end, world));
		}
	}

	// Color start node yellow
	colors.put(start, YELLOW);
	// Set start node's distance to 0
	costs.set(start.row, start.col, 0);
	// Enqueue start node
	pq.enqueue(start, heuristics.get(start.row, start.col));

	while (!pq.isEmpty()) {
		// Dequeue cheapest node
		Loc min = pq.dequeueMin();
		// Color that node green
		colors.put(min, GREEN);
		colorCell(world, min, GREEN);
		// If we arrived to destination, we've found the path
		if (min == end) break;
		// Iterate over all neighbours
		foreach (Loc l in getNeighbourSet(min, world.numRows(), world.numCols())) {
			// Calculate cost
			double cost = costFn(min, l, world);
			if (colors.get(l) == GRAY) {
				// Color node yellow
				colors.put(l, YELLOW);
				colorCell(world, l, YELLOW);
				// Set this node's cost to cost from start to its parent plus cost between them */
				costs.set(l.row, l.col, cost + costs.get(min.row, min.col));
				// Set this node's parent
				parents.put(l, min);
				// Enqueue this node
				pq.enqueue(l, costs.get(l.row, l.col) + heuristics.get(l.row, l.col));
			} else if (colors.get(l) == YELLOW && costs.get(l.row, l.col) > costs.get(min.row, min.col) + cost) {
				// Set cost
				costs.set(l.row, l.col, costs.get(min.row, min.col) + cost);
				// Set this node's parent
				parents.put(l, min);
				// Update priority
				pq.decreaseKey(l, costs.get(l.row, l.col) + heuristics.get(l.row, l.col));
			}
		}
	}

	// Restore shortest path
	Vector<Loc> answer;
	Loc curr = end;
	while (true) {
		answer.add(curr);
		if (curr == start) break;
		curr = parents.get(curr);
	}
	return answer;
}

/* This method just checks if specific locations' neighbours are in grid bounds and returns a set of such neighbours */
Set<Loc> getNeighbourSet(Loc loc, int xdim, int ydim) {
	Set<Loc> locations;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = loc.row + i;
			int y = loc.col + j;
			if (x >= 0 && x < xdim && y >= 0 && y < ydim) {
				Loc l;
				l.row = x;
				l.col = y;
				locations.add(l);
			}
		}
	}
	return locations;
}

Set<Edge> createMaze(int numRows, int numCols) {
	Set<Edge> spanningTree;
	Set<Edge> edgeSet;
	Vector< Set<Loc> > clusters;
	PriorityQueue<Edge> pq;

	// Search through all nodes and initialize graph 
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			// Create node
			Loc l;
			l.row = i;
			l.col = j;
			// Insert each node into its own cluster
			Set<Loc> cluster;
			cluster.add(l);
			clusters.add(cluster);
			// Get their neighbours, including diagonally positioned ones
			Set<Loc> neighbours = getNeighbourSet(l, numRows, numCols);
			// Add edges connecting current node to its neighbour in edgeSet excluding diagonal edges
			foreach (Loc nb in neighbours) {
				if (nb.col == l.col || nb.row == l.row) {
					// Create edge
					Edge e;
					e.start = l;
					e.end = nb;
					// Add it to set, because some of them are going to repeat
					edgeSet.add(e);
				}
			}
		}
	}
		
	// Insert all edges in the graph into a priority queue
	foreach (Edge e in edgeSet) {
		pq.enqueue(e, randomInteger(0, 9));
	}

	while (clusters.size() > 1) {
		Edge lightestEdge = pq.dequeue();
		// Variables to store each cluster's index in vector
		int firstIndex = 0;
		int secondIndex = 0;
		for (int i = 0; i < clusters.size(); i++) {
			if (clusters[i].contains(lightestEdge.start)) {
				firstIndex = i;
			}
			if (clusters[i].contains(lightestEdge.end)) {
				secondIndex = i;
			}
		}
		// If the endpoints of e are not in the same cluster
		if (firstIndex != secondIndex) {
			// Merge clusters
			clusters[firstIndex] += clusters[secondIndex];
			clusters.remove(secondIndex);
			// Add this edge to the spanning tree
			spanningTree.add(lightestEdge);
		}
	}

    return spanningTree;
}
