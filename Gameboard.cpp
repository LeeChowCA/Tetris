#include"Gameboard.h"

// fill the board with EMPTY_BLOCK 
//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
// - params: none
// - return: nothing
void Gameboard::empty() {
	for (int y = 0; y < MAX_Y; y++) {
		fillRow(y, EMPTY_BLOCK);
		//for (int x = 0; x < MAX_X; x++) {
			//grid[y][x] = -1;
			
		//}
	}
}

Gameboard::Gameboard() {
	empty();
}

// print the grid contents to the console (for debugging purposes)
//   use std::setw(2) to space the contents out 
//   (google it, you'll need to #include <iomanip>).
//   If the content == EMPTY_BLOCK, print a '.', otherwise print the content.
// - params: none
// - return: nothing
void Gameboard::printToConsole() const {
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			if (grid[y][x] == EMPTY_BLOCK) {
				std::cout << std::setw(2) << '.';
				//std::cout << ".";
			}
			else {
				std::cout << std::setw(2) << grid[y][x];
				//std::cout << grid[y][x];
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// Determine if a given Point is a valid grid location
// - param 1: a Point object
// - return: true if the point is a valid grid location, false otherwise
bool Gameboard::isValidPoint(const Point& p) const {
	return 0 <= p.getY() && p.getY() < MAX_Y && 0 <= p.getX() && p.getX() < MAX_X;
}

// Determine if a given XY is a valid grid location
// - param 1: an int representing x
// - param 2: an int representing y
// - return: true if the x,y is a valid grid location, false otherwise
bool Gameboard::isValidPoint(const int x, const int y) const {
	return 0 <= x && x < MAX_X && 0 <= y && y < MAX_Y;
}

// get the content at a given point 
// assert the point is valid (use isValidPoint())
// - param 1: a Point (the XY of the content weant to retrieve)
// - return: an int, the content from the grid at the specified point 
int Gameboard::getContent(const Point& p) const {
	//empty is -1
	assert(isValidPoint(p));
	
	return grid[p.getY()][p.getX()];
}

// get the content at a given XY location
// assert the point is valid (use isValidPoint())
// - param 1: an int for X (column)
// - param 2: an int for Y (row)
// - return: an int, the content from the grid at the specified XY
int Gameboard::getContent(const int x, const int y) const {
	assert(y < MAX_Y && x < MAX_X);

	if (isValidPoint(x, y)) {
		return grid[y][x];
	}
}


// set the content at a given point (ignore invalid points)
// - param 1: a Point
// - param 2: an int representing the content we want to set at this location.
void Gameboard::setContent(const Point& p, const int c) {
	if (isValidPoint(p)) {
		grid[p.getY()][p.getX()] = c;
	}
}

// set the content at an x,y position (ignore invalid points)
// - param 1: an int for X (column)
// - param 2: an int for Y (row)
// - param 3: an int representing the content we want to set at this location	
void Gameboard::setContent(int x,  int y,  int c) {
	if (isValidPoint(x, y)) {
		grid[y][x] = c;
	}
}

// set the content for a set of points (ignore invalid points)
// - param 1: a vector of Points representing locations
// - param 2: an int representing the content we want to set.
void Gameboard::setContent(const std::vector<Point>& pts,  int c) {
	for (Point p : pts) {
		setContent(p, c);
	}
}

// fill a given grid row with specified content
// - param 1: an int representing a row index
// - param 2: an int representing content
// - return: nothing
void Gameboard::fillRow( int row,  int content) {
	//need to pass the reference, otherwise point will be just a copy
	for (int& point : grid[row]) {
		point = content;
	}
}


// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
// assert the row index is valid
// - param 1: an int representing the row index we want to test
// - return: bool representing if the row is completed
bool Gameboard::isRowCompleted( int row) const {
	assert(row < MAX_Y);
	bool completed = true;

	for (int point : grid[row]) {
		if (point == EMPTY_BLOCK) {
			completed = false;
		}
	}

	return completed;
}

// Determine if (valid) all points passed in are empty
// *** IMPORTANT: Assume invalid x,y values can be passed to this method.
// Invalid meaning outside the bounds of the grid.
// * ONLY TEST VALID POINTS (use isValidPoint()). Ignore invalid Points.
// Using invalid points to index into the grid would cause undefined behaviour. 
// - param 1: a vector of Points representing locations to test
// - return: true if the content at ALL VALID points is EMPTY_BLOCK, false otherwise
bool Gameboard::areAllLocsEmpty(const std::vector<Point>& pts) const {
	bool allEmpty = true;

	for (Point p : pts) {
		if (isValidPoint(p)) {
			if (grid[p.getY()][p.getX()] != EMPTY_BLOCK) {
				allEmpty = false;
			}
		}
	}
	return allEmpty;
}

// Remove all completed rows from the board
//   use getCompletedRowIndices() and removeRows() 
// - params: none
// - return: the count of completed rows removed
int Gameboard::removeCompletedRows() {
	//std::cout << static_cast<int>(getCompletedRowIndices().size());
	int removedRowsCount{ static_cast<int>(getCompletedRowIndices().size()) };

	removeRows(getCompletedRowIndices());

	//need to do type conversion
	return removedRowsCount;
}

// A getter for the spawn location
// - params: none
// - returns: a Point, representing our private spawnLoc
Point Gameboard::getSpawnLoc() const {
	return spawnLoc;
}

// This is commented out to allow us to test. 

// scan the board for completed rows.
// Iterate through grid rows and use isRowCompleted(rowIndex)
// - params: none
// - return: a vector of completed row indices (integers).
std::vector<int> Gameboard::getCompletedRowIndices() const {
	std::vector<int> completedRows;

	for (int y = 0; y < MAX_Y; y++) {
		if (isRowCompleted(y)) {
			completedRows.push_back(y);
		}
	}
	return completedRows;
}

// copy a source row's contents into a target row.
// - param 1: an int representing the source row index
// - param 2: an int representing the target row index
// - return: nothing
void Gameboard::copyRowIntoRow( int sourceRow,  int targetRow) {
	for (int x = 0; x < MAX_X; x++) {
		grid[targetRow][x] = grid[sourceRow][x];
	}
}

// In gameplay, when a full row is completed (filled with content)
// it gets "removed".  To be exact, the row itself is not removed
// but the content from the row above it is copied into it.
// This continues all the way up the grid until the first row is copied
// into the second row.  Finally, the first row is filled with EMPTY_BLOCK
// Given a row index:
//   1) Assert the row index is valid
//   2) Starting at rowIndex, copy each row above the removed
//     row "one-row-downwards" in the grid.
//     (loop from y=rowIndex-1 down to 0, and copyRowIntoRow(y, y+1)).
//   3) call fillRow() on the first row (and place EMPTY_BLOCKs in it).
// - param 1: an int representing a row index
// - return: nothing
void Gameboard::removeRow( int rowIndex) {
	assert(rowIndex < MAX_Y);

	for (int y = rowIndex - 1; y >= 0; y--) {
		copyRowIntoRow(y, y + 1);
	}
	fillRow(0, EMPTY_BLOCK);
}

// given a vector of row indices, remove them. 
//   Iterate through the vector, call removeRow() on each row index. 
// - param 1: a vector of integers representing row indices we want to remove
// - return: nothing
void Gameboard::removeRows(const std::vector<int>& rowsToRemove) {
	for (int row : rowsToRemove) {
		removeRow(row);
	}
}