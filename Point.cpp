#include "Point.h"

int Point::getX () const{
	return x;
}


int Point::getY() const {
	return y;
}

void Point::setX(int x) {
	this->x = x;
}

void Point::setY(int y) {
	this->y = y;
}

void Point::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}

// swap x and y
void Point::swapXY(){
	int middleNum{};

	middleNum = this->x;
	this->x = this->y;
	this->y = middleNum;
}

// multiply x by some factor
void Point::multiplyX(int factor) {
	this->x = this->x * factor;
}

// multiply y by some factor
void Point::multiplyY(int factor) {
	this->y = this->y * factor;
}

// return a string in the form "[x,y]" to represent the state of the Point instance
// (for debugging) You could use stringstream if you want to try a forming a string in
// the same way you use cout https://www.learncpp.com/cpp-tutorial/stream-classes-for-strings/
std::string Point::toString() const {
	std::string s = "[" + std::to_string(x) + "," + std::to_string(y) + "]";

	return s;
}