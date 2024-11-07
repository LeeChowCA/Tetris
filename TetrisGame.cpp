#include "TetrisGame.h"

const int TetrisGame::BLOCK_WIDTH = 32;
const int TetrisGame::BLOCK_HEIGHT = 32;
const double TetrisGame::MAX_SECONDS_PER_TICK = 0.75;
const double TetrisGame::MIN_SECONDS_PER_TICK = 0.20;



// constructor
//   initialize/assign private member vars names that match param names
//   reset() the game
//   load font from file: fonts/RedOctober.ttf
//   setup scoreText
// - params: already specified
TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset)
	:window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset)
{
	reset();
	//currentShape.setGridLoc(board.getSpawnLoc());
	//should set the nextShape gridLoc into spawn location here or somewhere else to make sure it falls from the middle
	//nextShape.setGridLoc(board.getSpawnLoc());

	// setup our font for drawing the score
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
	{
		assert(false && "Missing font: RedOctober.ttf");
	};
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(18);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(425, 325);
}

// Draw anything to do with the game,
//   includes the board, currentShape, nextShape, score
//   called every game loop
// - params: none
// - return: nothing
void TetrisGame::draw() {
	Point* p = new Point(0, 0);
	TetColor t = TetColor::PURPLE;
	//drawBlock(*p, 1, 1,t );
	//int content = 2;
	//std::cout << content;
	//TetColor rContent = static_cast<TetColor>(content);
	//drawBlock(*p, 1, 1, rContent);

	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	window.draw(scoreText);


}

// Event and game loop processing
// handles keypress events (up, left, right, down, space)
// - param 1: sf::Event event
// - return: nothing
void TetrisGame::onKeyPressed(const sf::Event& event) {

	//still need to figure out why it's 1 and -1, instead of BLOCK_HEIGHT or BLOCK_WIDTH
	switch (event.key.code)
	{
	case sf::Keyboard::Up:
		attemptRotate(currentShape);
		break;
	case sf::Keyboard::Left:
		attemptMove(currentShape, -1, 0);
		break;
	case sf::Keyboard::Right:
		attemptMove(currentShape, 1, 0);
		break;
	case sf::Keyboard::Down:
		if (!attemptMove(currentShape, 0, 1)) {
			lock(currentShape);
		}
		break;
	case sf::Keyboard::Space:
		drop(currentShape);
		lock(currentShape);
		break;
	default:
		break;
	}

}

// called every game loop to handle ticks & tetromino placement (locking)
// - param 1: float secondsSinceLastLoop
// return: nothing
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick += secondsSinceLastLoop;

	if (secondsSinceLastTick - secondsPerTick > 0) {
		tick();
		secondsSinceLastTick -= secondsPerTick;
	}

	if (shapePlacedSinceLastGameLoop) {
		shapePlacedSinceLastGameLoop = false;
		if (spawnNextShape()) {
			pickNextShape();
			int rowsRemoved = board.removeCompletedRows();
			std::cout << "removed:" << rowsRemoved<<"\n";
			switch (rowsRemoved)
			{
			case 1:
				score += 40;
				std::cout << "removed one\n";
				updateScoreDisplay();
				break;
			case 2:
				score += 100;
				updateScoreDisplay();
				break;
			case 3:
				score += 300;
				updateScoreDisplay();
				break;
			case 4:
				score += 1200;
				updateScoreDisplay();
				break;
			default:
				break;
			}

			determineSecondsPerTick();
		}
		else
		{
			std::cout << "failed";
			reset();
		}
		
	}
}

// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further).
// - params: none
// - return: nothing
void TetrisGame::tick() {
	int counter{ 0 };

	if (attemptMove(currentShape, 0, 1)) {
		
	}
	else {
		lock(currentShape);
	}

	//std::cout << counter++;
}

// reset everything for a new game (use existing functions) 
//  - set the score to 0 and call updateScoreDisplay()
//  - call determineSecondsPerTick() to determine the tick rate.
//  - clear the gameboard,
//  - pick & spawn next shape
//  - pick next shape again (for the "on-deck" shape)
// - params: none
// - return: nothing
void TetrisGame::reset() {
	score = 0;
	updateScoreDisplay();
	determineSecondsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();

}


// assign nextShape.setShape a new random shape  
// - params: none
// - return: nothing
void TetrisGame::pickNextShape() {
	nextShape.setShape(Tetromino::getRandomShape());
	//nextShape.setGridLoc(nextShapeOffset);
}

// copy the nextShape into the currentShape (through assignment)
//   position the currentShape to its spawn location.
// - params: none
// - return: bool, true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() {
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());

	return isPositionLegal(currentShape);
}

// Test if a rotation is legal on the tetromino and if so, rotate it. 
//  To accomplish this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (temp.rotateClockwise())
//	 3) test if temp rotation was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
// - param 1: GridTetromino shape
// - return: bool, true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape) {
	GridTetromino tempShape = shape;

	tempShape.rotateClockwise();

	if (isPositionLegal(tempShape)) {
		shape.rotateClockwise();
		return true;
	}
	return false;
}

// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.	
// - param 1: GridTetromino shape
// - param 2: int x;
// - param 3: int y;
// - return: true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
	GridTetromino tempShape = shape;

	tempShape.move(x, y);

	//std::cout << "attempt to move:" << isPositionLegal(tempShape) << "\n";
	//for (Point p : shape.getBlockLocsMappedToGrid()) {
	//	std::cout << "actucally shapote"<<p.getY() << "\n";
	//}
	//std::cout << shape.getGridLoc().getY() << "\n";
	if (isPositionLegal(tempShape)) {
		if (!shapePlacedSinceLastGameLoop) {
			shape.move(x, y);
			return true;
		}
		
	}
	return false;
}

// drops the tetromino vertically as far as it can 
//   legally go.  Use attemptMove(). This can be done in 1 line.
// - param 1: GridTetromino shape
// - return: nothing;
void TetrisGame::drop(GridTetromino& shape) {
	//make sure we move step by step. because there might be a block under gridLoc.
	while (attemptMove(shape, 0, 1));
	//attemptMove(shape, 0, shape.getGridLoc().getY() + (board.MAX_Y - shape.getGridLoc().getY() - 3));
}


// copy the contents (color) of the tetromino's mapped block locs to the grid.
	//	 1) get the tetromino's mapped locs via tetromino.getBlockLocsMappedToGrid()
	//   2) use the board's setContent() method to set the content at the mapped locations.
	//   3) record the fact that we placed a shape by setting shapePlacedSinceLastGameLoop
	//      to true
	// - param 1: GridTetromino shape
	// - return: nothing
void TetrisGame::lock(const GridTetromino& shape) {
	std::vector<Point> blocks = shape.getBlockLocsMappedToGrid();

	for (Point block : blocks) {
		board.setContent(block.getX(), block.getY(), static_cast<int>(shape.getColor()));
	}

	shapePlacedSinceLastGameLoop = true;
	
}

// Graphics methods ==============================================

// Draw a tetris block sprite on the canvas		
// The block position is specified in terms of 2 offsets: 
//    1) the top left (of the gameboard in pixels)
//    2) an x & y offset into the gameboard - in blocks (not pixels)
//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
//       to get the pixel offset.
//	 1) set the block color using blockSprite.setTextureRect()
//   2) set the block location using blockSprite.setPosition()   
//	 3) draw the block using window.draw()
//   For details/instructions on these 3 operations see:
//       www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
//       use member variables: window and blockSprite (assigned in constructor)
// param 1: Point topLeft
// param 2: int xOffset
// param 3: int yOffset
// param 4: TetColor color
// return: nothing
void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, const TetColor& color) {
	sf::Texture texture;
	texture.loadFromFile("images/tiles.png");

	//0,0 is the coordinate of the top left of the image.
	if (!texture.loadFromFile("images/tiles.png")) {
		std::cout << "image file fails loading";
	}

	sf::Sprite blockSprite;
	//assign the texture to use
	blockSprite.setTexture(texture);

	switch (color)
	{
	case TetColor::RED:
		//decide which part of the texture to use, here, wo use the part from topleft to (BLOCK_WIDTH, BLOCK_HEIGHT)
		blockSprite.setTextureRect(sf::IntRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		break;
	case TetColor::ORANGE:
		blockSprite.setTextureRect(sf::IntRect(0 + BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		break;
	case TetColor::YELLOW:
		blockSprite.setTextureRect(sf::IntRect(0 + BLOCK_WIDTH * 2, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		break;
	case TetColor::GREEN:
		blockSprite.setTextureRect(sf::IntRect(0 + BLOCK_WIDTH * 3, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		break;
	case TetColor::BLUE_LIGHT:
		blockSprite.setTextureRect(sf::IntRect(0 + BLOCK_WIDTH * 4, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		break;
	case TetColor::BLUE_DARK:
		blockSprite.setTextureRect(sf::IntRect(0 + BLOCK_WIDTH * 5, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		break;
	case TetColor::PURPLE:
		blockSprite.setTextureRect(sf::IntRect(0 + BLOCK_WIDTH * 6, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		break;
	default:
		std::cout<<"wrong color to choose";
		break;
	}

	//set the postion of the blockSprite on the board. gameboardOffset will help to navigate to the topleft of gameboard
	blockSprite.setPosition(topLeft.getX() + xOffset * BLOCK_WIDTH, topLeft.getY() + yOffset * BLOCK_HEIGHT);
	//draw the sprite on the board, need to be after setposition function.
	window.draw(blockSprite);
}

// Draw the gameboard blocks on the window
//   Iterate through each row & col, use drawBlock() to 
//   draw a block if it isn't empty.
// params: none
// return: nothing
void TetrisGame::drawGameboard() {

	Point* p = new Point(0, 0);
	//TetColor t = TetColor::BLUE_DARK;
	//board.setContent(5,5,2);
	

	for (int y = 0; y < board.MAX_Y; y++) {
		for (int x = 0; x < board.MAX_X; x++) {
			if (board.getContent(x, y) != Gameboard::EMPTY_BLOCK) {
				int content = board.getContent(x, y);
				//std::cout << content;
				TetColor rContent = static_cast<TetColor>(content);
				//std::cout << static_cast<int>(rContent);
				drawBlock(gameboardOffset, x, y, rContent);
			}
		}
	}
}

// Draw a tetromino on the window
//	 Iterate through each mapped loc & drawBlock() for each.
//   The topLeft determines a 'base point' from which to calculate block offsets
//      If the Tetromino is on the gameboard: use gameboardOffset
// param 1: GridTetromino tetromino
// param 2: Point topLeft
// return: nothing
void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft) {
	std::vector<Point> mapLocs = tetromino.getBlockLocsMappedToGrid();
	TetColor t = TetColor::GREEN;

	if (tetromino.getGridLoc().getX() < board.MAX_X && tetromino.getGridLoc().getY() < board.MAX_Y) {
		for (Point p : mapLocs) {
            TetColor color = tetromino.getColor();
			TetColor& rColor = color;
            drawBlock(topLeft, p.getX(), p.getY(), rColor);
		}
	}
}

// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
// params: none:
// return: nothing
void TetrisGame::updateScoreDisplay() {
	std::string scoreString = "score: " + std::to_string(score);
	std::cout << "current score: " << score << "\n";

	scoreText.setString(scoreString);
}

// State & gameplay/logic methods ================================

// Determine if a Tetromino can legally be placed at its current position
// on the gameboard.
//   Tip: Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs.
// - param 1: GridTetromino shape
// - return: bool, true if shape is within borders (isWithinBorders()) and 
//           the shape's mapped board locs are empty (false otherwise).
bool TetrisGame::isPositionLegal(const GridTetromino& shape) const {
	//std::vector<Point> blocks = shape.getBlockLocsMappedToGrid();


	//for (Point p : shape.getBlockLocsMappedToGrid()) {
	//	std::cout << "ispositionleag"<<p.getY() << "\n";
	//}
	//for (Point& block : shape.getBlockLocsMappedToGrid()) {
	//	if (block.getY() >= board.MAX_Y ) {
	//		std::cout << block.getY()<<"\n";
	//		return false;
	//	}
	//}
	 

	return board.areAllLocsEmpty(shape.getBlockLocsMappedToGrid()) && isWithinBorders(shape);
}

// Determine if the shape is within the left, right, & bottom gameboard borders
//   * Ignore the upper border because we want shapes to be able to drop
//     in from the top of the gameboard.
//   All of a shape's blocks must be inside these 3 borders to return true
// - param 1: GridTetromino shape
// - return: bool, true if the shape is within the left, right, and lower border
//	         of the grid, but *NOT* the top border (false otherwise)
bool TetrisGame::isWithinBorders(const GridTetromino& shape) const {

	//if (shape.getShape() == TetShape::S || shape.getShape() == TetShape::Z || shape.getShape() == TetShape::T) {
	//	
	//}

	bool isWithin = true;

	std::vector<Point> blocks = shape.getBlockLocsMappedToGrid();

	for (Point block : blocks) {
		if (!(block.getX() >= 0) || !(block.getX() < board.MAX_X) || !(block.getY() < board.MAX_Y)) {
			return false;
		}
	}

	//if(shape.getGridLoc().getX() < board.MAX_X - 1 && shape.getGridLoc().getX() > 0){
	//	std::cout << board.MAX_X << shape.getGridLoc().getX() << "\n";
	//	return true;
	//}

	return isWithin;
}

// set secsPerTick 
//   - basic: use MAX_SECS_PER_TICK
//   - advanced: base it on score (higher score results in lower secsPerTick)
// params: none
// return: nothing
void TetrisGame::determineSecondsPerTick() {
	
}