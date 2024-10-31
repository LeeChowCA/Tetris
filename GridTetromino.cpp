#include"GridTetromino.h"

GridTetromino::GridTetromino() {
	gridLoc.setXY(0, 0);
}


Point GridTetromino::getGridLoc() const {
	return gridLoc;
}


void GridTetromino::setGridLoc(int x, int y) {
	gridLoc.setXY(x, y);
}

void GridTetromino::setGridLoc(Point p) {
	gridLoc.setXY(p.getX(), p.getY());
}

void GridTetromino::move(int xOffset, int yOffset) {
	gridLoc.setXY(gridLoc.getX() + xOffset, gridLoc.getY() + yOffset);
}

std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const {
	std::vector<Point> mappedVec{};

	for (Point p : blockLocs) {
		//set each element of t he blockLocs to the mapped position and then push it to the mappedVec vector.
		p.setXY(p.getX() + gridLoc.getX(), p.getY() + gridLoc.getY());
		mappedVec.push_back(p);
	}

	return mappedVec;
	//std::vector<Point> blockLocs = Tetromino::getShape();
}