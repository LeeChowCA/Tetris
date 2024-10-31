#ifndef POINT_H
#define POINT_H

#include<string>

class Point {
	friend class TestSuite;
private:
	int x;
	int y;
public:
	//default constructor
	Point() {
		x = 0;
		y = 0;
	}

	//constructor overloading
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	//get the coordinate of X
	int getX() const;

	//get the coordinate of Y
	int getY() const;

	//set the coordinate of X
	void setX(int x);

	//set the coordinate of Y
	void setY(int y);

	//set the coordinate of both X and Y
	void setXY(int x, int y);

	// swap x and y
	void swapXY();

	// multiply x by some factor
	void multiplyX(int factor);

	// multiply y by some factor
	void multiplyY(int factor);

	// return a string in the form "[x,y]" to represent the state of the Point instance
	// (for debugging) You could use stringstream if you want to try a forming a string in
	// the same way you use cout https://www.learncpp.com/cpp-tutorial/stream-classes-for-strings/
	std::string toString() const;


};
#endif

