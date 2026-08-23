#include "BigGhost.h"
#include "Board.h"
#include "Tiles.h"

void BigGhost::setDirection() { // "Sophisticated algorithm that tries to hunt Mario"
		currDirY = 0;
	if (y != marioY) {
		if (!tryClimb()) {
			if (x < marioX)
				currDirX = 1;
			else if (x == marioX)
				currDirX = 0;
			else // Case: Mario is behind the ghost
				currDirX = -1;
		}
	}

	else {// Case: Ghost is chasing mario on the current floor
		char ChBelow = board.getBoardChar(x, y + 1);

		if (x < marioX && (Tiles::isTile(ChBelow) || ChBelow == Board::WALL))
			currDirX = 1;
		else if (x > marioX && (Tiles::isTile(ChBelow) || ChBelow == Board::WALL))
			currDirX = -1;
		else if (x == marioX)
			currDirX = 0;
	}
}

bool BigGhost::tryClimb() {

	if (y > marioY && onLadder) { // Case: Mario is above the ghost
		currDirY = -1;
		currDirX = 0;
		return true;
	}
	if (y < marioY && onLadder) { // Case: Mario is below the ghost
		currDirY = 1;
		currDirX = 0;
		return true;
	}

	return false; // Case: couldn't climb a ladder
}

void BigGhost::updateState() {
	char chBelowBelow = board.getBoardChar(x, y + 2); // Char below^2 Ghost
	char chBelow = board.getBoardChar(x, y + 1);  // Char below Ghost
	char chAbove = board.getBoardChar(x, y - 1);  // Char above Ghost

	if (chBelow == Board::LADDER || (chAbove == Board::LADDER && y < marioY))  // Case: on a ladder, or should climb up
		onLadder = true;
	else if ((Tiles::isTile(chBelow) || chBelow == Board::WALL) && prevCh != Board::LADDER) // Case: Ghost is on a valid floor
		onLadder = false;
	else if (prevCh == Board::LADDER && y > marioY) // Case: Climbing one-step ladder
		onLadder = true;
	else if (chBelow != Board::LADDER && y <= marioY) // Case: Done descending on the current ladder
		onLadder = false;

	if ((Tiles::isTile(prevCh) || prevCh == Board::WALL) && (chAbove == Board::EMPTY || chBelow == Board::LADDER)) // Case: Ghost is climbing and its prevCh is still a floor
		onLadder = true;

	if (prevCh == Board::LADDER && chBelow != Board::LADDER && chBelowBelow != Board::LADDER && y < marioY) // Case: Ghost is in the bottom of the ladder but should not climb
		onLadder = false;

	if (y < marioY && Tiles::isTile(chBelow) && board.isWithinBounds(x, y + 2) && board.getBoardChar(x, y + 2) == Board::LADDER)
		onLadder = true;
}

void BigGhost::move() {
	calculatePrevPos();
	eraseFromBoard();
	eraseFromConsole();

	updateState();
	setDirection();

	executeMove(isValidToMove());
	drawToBoard();
	drawToConsole();
}

bool BigGhost::isValidToMove() {
	int nextPosX = x + currDirX;
	int nextPosY = y + currDirY;

	if (!board.isWithinBounds(nextPosX, nextPosY))
		return false;

	// Ledge detection
	if (currDirX != 0 && board.isWithinBounds(nextPosX, nextPosY + 1)) {
		char chBelowNext = board.getBoardChar(nextPosX, nextPosY + 1);

		if (chBelowNext == Board::EMPTY)
			return false;
	}

	char nextCh = board.getBoardChar(nextPosX, nextPosY);
	char chBelow = board.getBoardChar(x, y + 1);

	// Allow Big Ghost to climb through floor tiles
	if (Tiles::isTile(nextCh)) {
		if (currDirY == -1 && onLadder) // Case: climbing up through a floor
			return true;
		if (currDirY == 1 && Tiles::isTile(chBelow) && board.isWithinBounds(x, y + 2) && board.getBoardChar(x, y + 2) == Board::LADDER) // Climbing down through a floor
			return true;

		return false; // Big Ghost cannot walk horizontally into tiles
	}

	return Enemy::isValidToMove();
}