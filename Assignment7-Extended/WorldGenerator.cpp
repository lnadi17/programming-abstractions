/******************************************************************************
 * File: WorldGenerator.h
 *
 * Code for generating the terrains and mazes in which paths will be computed.
 */

#include "WorldGenerator.h"
#include "Trailblazer.h"
#include "TrailblazerConstants.h"
#include "vector.h"
#include "set.h"
#include "random.h"
#include <cmath>
//#include <algorithm>
//#include <limits>
using namespace std;

/*** Internal constants ***/

const int CELL_COUNT = 10;

/* Variance used by the Gaussian blur step. */
const double kSigma = 1.0;

/* Pi, more or less. */
const double kPi = 3.1415926535897932384626433;

/* The size of the window in which our Gaussian kernel will be applied.
 * Wikipedia suggest uses a window whose size is six times the variance, since
 * that amount should capture most of the distribution.
 */
const int kWindowSize = int(kSigma * 6.0 + 0.5);

/*** Custom types ***/

struct Vec {
		int x;
		int y;
};

struct UnitVec {
	double x;
	double y;
};

/*** Function prototypes ***/
Vec getRandomVector(int xLen, int yLen);
double dotProduct(UnitVec v1, UnitVec v2);
double lerp(double a, double b, double w);
double perlin(int row, int col, Grid<Vec> & vectorGrid, int xLen, int yLen);
static Grid<double> wallsToGrid(Set<Edge>& walls, int numRows, int numCols);
UnitVec normalize(Vec v);
double mapAverage(double val);
double fade(double t);
UnitVec normalize(UnitVec v);
static void smoothTerrain(Grid<double>& terrain);
static Grid<double> createGaussianKernel();

/*** Function implementations ***/

/* Generates a random terrain of the specified size. */
Grid<double> generateRandomTerrain(int numRows, int numCols) {
	Grid<double> heights(numRows, numCols);

	// Create gradient vector
	Grid<Vec> vectorGrid(CELL_COUNT + 1, CELL_COUNT + 1);

	// Calculate cell lengths
	int xLen = (numRows % CELL_COUNT == 0 ? numRows / CELL_COUNT : numRows / CELL_COUNT + 1);
	int yLen = (numCols % CELL_COUNT == 0 ? numCols / CELL_COUNT : numCols / CELL_COUNT + 1);

	// Generate random vectors
	for (int i = 0; i < CELL_COUNT + 1; i++) {
		for (int j = 0; j < CELL_COUNT + 1; j++) {
			vectorGrid.set(i, j, getRandomVector(xLen, yLen));
		}
	}

	// Run perlin function
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			heights.set(i, j, perlin(i, j, vectorGrid, xLen, yLen));
		}
	}
	
	smoothTerrain(heights);
	return heights;
}

// Generates random vector which, in each direction, has limited length
Vec getRandomVector(int xLen, int yLen) {
	Vec vector;
	vector.x = randomInteger(1 - xLen, xLen - 1);
	vector.y = randomInteger(1 - xLen, yLen - 1);
	return vector;
}

/* Computes perlin function for given coordinates */
double perlin(int row, int col, Grid<Vec> & vectorGrid, int xLen, int yLen) {
	// Determine grid cell coordinates
	int x0 = row / xLen;
	int y0 = col / yLen;
	// Determine point's coordinates inside the grid
	double x = row % xLen;
	double y = col % yLen;
	if (x == 0) x = randomReal(0, 1);
	if (x == 12) x = randomReal(11, 12);
	if (y == 0) y = randomReal(0, 1);
	if (y == 12) y = randomReal(11, 12);
	// Determine influence values where d1 ... d4 are distance vectors and v1 ... v4 are gradient vectors
	// [g1] | [g2]
	// [g3] | [g4]
	Vector<double> dotProducts;
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= 1; j++) {
			// Get gradient vector
			Vec v = vectorGrid.get(x0 + j, y0 + i);
			// Compute distance vector
			UnitVec d;
			d.x = x - j * (xLen - 1);
			d.y = y - i * (yLen - 1);
			
			// Compute dot product
			double dotProd = dotProduct(normalize(v), normalize(d));
			dotProducts.add(dotProd);
		}
	}
	
	// Determine interpolation weights (value should be from 0 to 1)
	double wx = double(x) / double(xLen - 1);
	double wy = double(y) / double(yLen - 1);
	
	double x1 = lerp(dotProducts.get(0), dotProducts.get(1), fade(wx));
	double x2 = lerp(dotProducts.get(2), dotProducts.get(3), fade(wx));

	double average = lerp(x1, x2, fade(wy));

	return mapAverage(average);
}

double dotProduct(UnitVec v1, UnitVec v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}

// Converts number from [-1, 1] to [0, 1]
double mapAverage(double val) {
	return (val + 1) / 2;
}

double fade(double t) {
	// Original fade function defined bu Ken Perlin
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double a, double b, double w) {
	return ((1 - w) * a + w * b);
}

UnitVec normalize(Vec v)  {
	double length = sqrt(double(v.x * v.x + v.y * v.y));
	UnitVec u;
	u.x = double(v.x) / (length + 0.02);
	u.y = double(v.y) / (length + 0.02);
	return u;
}

UnitVec normalize(UnitVec v)  {
	double length = sqrt(double(v.x * v.x + v.y * v.y));
	UnitVec u;
	u.x = double(v.x) / (length + 0.02);
	u.y = double(v.y) / (length + 0.02);
	return u;
}

/* Uses the user-provided maze creation routine to generate a maze and then
 * render it into a grid.
 */
Grid<double> generateRandomMaze(int numRows, int numCols) {
	Set<Edge> maze = createMaze(numRows, numCols);
	return wallsToGrid(maze, numRows, numCols);
}

/* Given a set of walls, converts those walls into a 2D grid. */
static Grid<double> wallsToGrid(Set<Edge>& walls, int numRows, int numCols) {
  /* We need numRows - 1 interstitial blocks and numRows - 1 interstitial
   * blocks.
   */
  Grid<double> result(2 * numRows - 1, 2 * numCols - 1);
	
  /* Fill everything in. */
  for (int i = 0; i < result.numRows(); i++) {
    for (int j = 0; j < result.numCols(); j++) {
      result[i][j] = kMazeWall;
    }
  }
	
  /* Clear all cells corresponding to grid points. */
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      result[2 * i][2 * j] = kMazeFloor;
    }
  }
	
  /* Clear all interstitial cells corresponding to connections. */
  foreach (Edge wall in walls) {
    /* Confirm the endpoints are in-bounds. */
    if (!result.inBounds(wall.start.row, wall.start.col) ||
        !result.inBounds(wall.end.row, wall.end.col)) {
      error("Edge endpoints are out of range.");
    }

    int row = 2 * wall.start.row + (wall.end.row - wall.start.row);
    int col = 2 * wall.start.col + (wall.end.col - wall.start.col);
    result[row][col] = kMazeFloor;
  }
	
  return result;
}

/* Applies a Gaussian blur to the maze. */
static void smoothTerrain(Grid<double>& terrain) {
	Grid<double> kernel = createGaussianKernel();
	Grid<double> result(terrain.numRows(), terrain.numCols());
	
  /* Convolve the two matrices together.  For each point in the original grid,
   * compute the weighted sum of the Gaussian kernel applied at that point, then
   * write the result back to the resulting image.
   */
	for (int i = 0; i < terrain.numRows(); i++) {
		for (int j = 0; j < terrain.numCols(); j++) {
      /* We might not apply the entire kernel if we're on a border, so track
       * how much total weight was used.
       */
			double totalWeightUsed = 0.0;
			for (int a = 0; a < kernel.numRows(); a++) {
				for (int b = 0; b < kernel.numCols(); b++) {
          /* Determine the sample point from the original image.  If it's not
           * in-bounds, skip it.
           */
					int sampleRow = i + a - kernel.numRows() / 2;
					int sampleCol = j + b - kernel.numRows() / 2;
					if (!terrain.inBounds(sampleRow, sampleCol)) continue;
					
					totalWeightUsed += kernel[a][b];
					result[i][j] += kernel[a][b] * terrain[sampleRow][sampleCol];
				}
			}
			result[i][j] /= totalWeightUsed;
		}
	}
	
	terrain = result;
}

/* Creates a kernel matrix that can be used for a Gaussian blur. */
static Grid<double> createGaussianKernel() {
	Grid<double> result(kWindowSize, kWindowSize);
	for (int i = 0; i < kWindowSize; i++) {
		for (int j = 0; j < kWindowSize; j++) {
      /* We want to compute
       *
       * (1 / sqrt(2 pi sigma) * e^{(x^2 + y^2) / 2(sigma^2)}
       *
       * Sorry it's messy!
       */
			result[i][j] = exp(-(pow(i - kWindowSize / 2.0, 2.0) +
													 pow(j - kWindowSize / 2.0, 2.0)) / 
                         (2 * kSigma * kSigma)) /
                    sqrt(2 * kPi * kSigma);
		}
	}
	return result;
}