// The Gameboard class encapsulates the functionality of the Tetris game board.
//
// The Gameboard class has no concept of what a tetromino is!
// This is intentional.  It helps to keep the gameboard as modular and as simple
// as possible. The gameboard class simply manages a 2D array of content
// (integers). Each integer of content can represent either an empty spot, or a
// color.  Gameboard member functions operate on that content (particularly on
// rows) to facilitate operations that happen on a Tetris gameboard (checking for
// a complete row, filling a row, copying a row, collapsing rows, etc…).
//
// Actual gameplay is handled by a separate class (which makes use of the gameboard).
// We will handle this in a future lab. To get a sense of the bigger picture and how 
// the gameboard is used : a GridTetromino (a special Tetromino that has knowledge of
// its location on a co-ordinate system) will eventually reach a location on the 
// gameboard where it cannot move down any further (because the points that it would
// occupy already contain content).  At this point, the information a Tetromino 
// contains about its blocks (location and color) will be copied to the gameboard grid.
// We use the gameboard to determine what locations are already occupied as a result of 
// previous tetromino being “locked” (where its blocks are copied to the gameboard).
// 
// Displaying the board(via text output) should show empty spots, and areas where blocks
// of color(content) have been copied("locked") onto the board from tetrominos that have
// already been placed(either intentionally or not).
//
// - The game board is represented by a 2D array of integers(the grid).
// - The array contains content(integers) which represent either :
//    - an EMPTY_BLOCK(-1),
//    - a color from the Tetromino::TetColor enum.
// - This 2D array is oriented with [0][0] at the top left and [MAX_Y][MAX_X] at the
//      bottom right.  Why?  It makes the board much easier to draw the grid on the 
//      screen later because this is the same way things are drawn on a screen 
//      co-ordinate system (where pixel 0,0 is considered top left).
// - A 2D array uses row - major ordering by default – which means that the first index
//      is the row and the second is the column.eg: grid[row][col]. However it will be
//      easier for us to think of this grid in terms of XY coordinates. Here’s the 
//      catch… X represents columns and Y represents rows.  (I know this seems counter-
//      intuitive, but to make sense of it - think of pixels on a screen, or XY 
//      co-ordinates on a co-ordinate system)  
// - This means that when we index into a 2D array with an x, y co - ordinate, we are 
//      going to need to reverse what we would probably do naturally : (grid[x][y]), 
//      and index the grid the other way around(grid[y][x]). We will only need to do 
//      this within the gameboard class (since grid is a private member variable). 
//		We are using abstraction to hide the details of implementation inside the class.
//      If we call member functions that are public (eg : setContent(x, y, content))
//      we can treat X and Y as we normally would,
//
//  [expected .cpp size: ~ 225 lines (including method comments)]


#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include "Point.h"
#include<iostream>
#include "Tetromino.h"
#include <iomanip>
#include <cassert>

class Gameboard
{
	friend int main();
	friend class TestSuite;
public:
	// CONSTANTS
	static const int MAX_X = 10;		// gameboard x dimension
	static const int MAX_Y = 19;		// gameboard y dimension
	static const int EMPTY_BLOCK = -1;	// contents of an empty block

private:
	// MEMBER VARIABLES -------------------------------------------------

	// the gameboard - a grid of X and Y offsets.  
	//  ([0][0] is top left, [MAX_Y-1][MAX_X-1] is bottom right) 
	int grid[MAX_Y][MAX_X];
	// the gameboard offset to spawn a new tetromino at.
	const Point spawnLoc{ MAX_X / 2, 0 };
	
public:	
	// METHODS -------------------------------------------------
	// 
	// constructor - empty() the grid
	Gameboard();

	// fill the board with EMPTY_BLOCK 
	//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
	// - params: none
	// - return: nothing
	void empty();

	// print the grid contents to the console (for debugging purposes)
	//   use std::setw(2) to space the contents out 
	//   (google it, you'll need to #include <iomanip>).
	//   If the content == EMPTY_BLOCK, print a '.', otherwise print the content.
	// - params: none
	// - return: nothing
	void printToConsole() const;

	// get the content at a given point 
	// assert the point is valid (use isValidPoint())
	// - param 1: a Point (the XY of the content weant to retrieve)
	// - return: an int, the content from the grid at the specified point 
	int getContent(const Point& p) const;

	// get the content at a given XY location
	// assert the point is valid (use isValidPoint())
	// - param 1: an int for X (column)
	// - param 2: an int for Y (row)
	// - return: an int, the content from the grid at the specified XY
	int getContent(const int x, const int y) const;			

	// set the content at a given point (ignore invalid points)
	// - param 1: a Point
	// - param 2: an int representing the content we want to set at this location.
	void setContent(const Point& p, const int c);

	// set the content at an x,y position (ignore invalid points)
	// - param 1: an int for X (column)
	// - param 2: an int for Y (row)
	// - param 3: an int representing the content we want to set at this location	
	void setContent(const int x, const int y, const int c);
	
	// set the content for a set of points (ignore invalid points)
	// - param 1: a vector of Points representing locations
	// - param 2: an int representing the content we want to set.
	void setContent(const std::vector<Point>& pts, const int c);

	// Determine if (valid) all points passed in are empty
	// *** IMPORTANT: Assume invalid x,y values can be passed to this method.
	// Invalid meaning outside the bounds of the grid.
	// * ONLY TEST VALID POINTS (use isValidPoint()). Ignore invalid Points.
	// Using invalid points to index into the grid would cause undefined behaviour. 
	// - param 1: a vector of Points representing locations to test
	// - return: true if the content at ALL VALID points is EMPTY_BLOCK, false otherwise
	bool areAllLocsEmpty( const std::vector<Point>& pts) const;

	// Remove all completed rows from the board
	//   use getCompletedRowIndices() and removeRows() 
	// - params: none
	// - return: the count of completed rows removed
	int removeCompletedRows();

	// A getter for the spawn location
	// - params: none
	// - returns: a Point, representing our private spawnLoc
	Point getSpawnLoc() const;

private:  // This is commented out to allow us to test. 

	// Determine if a given Point is a valid grid location
	// - param 1: a Point object
	// - return: true if the point is a valid grid location, false otherwise
	bool isValidPoint(const Point& p) const;

	// Determine if a given XY is a valid grid location
	// - param 1: an int representing x
	// - param 2: an int representing y
	// - return: true if the x,y is a valid grid location, false otherwise
	bool isValidPoint(const int x, const int y) const;

	// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
	// assert the row index is valid
	// - param 1: an int representing the row index we want to test
	// - return: bool representing if the row is completed
	bool isRowCompleted(const int row) const;
	
	// fill a given grid row with specified content
	// - param 1: an int representing a row index
	// - param 2: an int representing content
	// - return: nothing
	void fillRow(const int row, const int content);
	
	// scan the board for completed rows.
	// Iterate through grid rows and use isRowCompleted(rowIndex)
	// - params: none
	// - return: a vector of completed row indices (integers).
	std::vector<int> getCompletedRowIndices() const;				

	// copy a source row's contents into a target row.
	// - param 1: an int representing the source row index
	// - param 2: an int representing the target row index
	// - return: nothing
	void copyRowIntoRow(const int sourceRow, const int targetRow);
		
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
	void removeRow(const int row);

	// given a vector of row indices, remove them. 
	//   Iterate through the vector, call removeRow() on each row index. 
	// - param 1: a vector of integers representing row indices we want to remove
	// - return: nothing
	void removeRows(const std::vector<int>& rowsToRemove);
};

#endif /* GAMEBOARD_H */

