#include "Tetromino.h"
#include <iostream>

TetColor Tetromino::getColor() const{
	return color;
}

TetShape Tetromino::getShape() const{
	return shape;
}

void Tetromino::setShape(TetShape shape) {
	this->shape = shape;

	switch (this->shape) {
		//Point has 2 private class variables.
		case TetShape::S:
			blockLocs = { Point(-1,0), Point(0,0), Point(0,1), Point(1,1) };
			color = TetColor::RED;
			break;
		case TetShape::Z:
			blockLocs = { Point(-1,1), Point(0,1), Point(0,0), Point(1,0) };
			color = TetColor::GREEN;
			break;
		case TetShape::L:
			blockLocs = { Point(0,2), Point(0,1), Point(0,0), Point(1,0) };
			color = TetColor::ORANGE;
			break;
		case TetShape::J:
			blockLocs = { Point(-1,-1), Point(0,-1), Point(0,0), Point(0,1) };
			color = TetColor::BLUE_DARK;
			break;
		case TetShape::O:
			blockLocs = { Point(0,0), Point(0,1), Point(1,1), Point(1,0) };
			color = TetColor::YELLOW;
			break;
		case TetShape::I:
			blockLocs = { Point(0,-1), Point(0,0), Point(0,1), Point(0,2) };
			color = TetColor::BLUE_LIGHT;
			break;
		case TetShape::T:
			blockLocs = { Point(-1,0), Point(0,0), Point(0,-1), Point(1,0) };
			color = TetColor::PURPLE;
			break;
	}
}

void Tetromino::printToConsole() const {
	//use x and y, don't use i and j, easy to get confused
	for (int y = 3; y > -3; y--) {
		for (int x = -3; x < 3; x++) {
			bool isBlock = true;

			for (auto blockLoc : blockLocs) {
				if (blockLoc.getX() == x && blockLoc.getY() == y) {
					isBlock = false;
				}
			}

			if (isBlock) {
				std::cout << ".";
			}
			else
				std::cout << "x";
		}
		std::cout << std::endl;
	}
}


void Tetromino::rotateClockwise() {
	for (auto& blockLoc : blockLocs) {
		if (shape != TetShape::O) {
			if (blockLoc.getX() != 0 || blockLoc.getY() != 0) {
				blockLoc.setXY(blockLoc.getY(), -blockLoc.getX());
			}
		}
	}
}



TetShape Tetromino::getRandomShape() {

	int randNum = rand() % static_cast<int>(TetShape::COUNT);

	return static_cast<TetShape>(randNum);
}