#include "SmallGhost.h"
#include "Board.h"

bool SmallGhost::isValidToMove() {
	int nextPosX = x + currDirX;
	int nextPosY = y + currDirY;

	if (!board.isWithinBounds(nextPosX, nextPosY))
		return false;

	// Ledge Detection
	if (board.isWithinBounds(nextPosX, nextPosY + 1)) {
		char chBelowNext = board.getBoardChar(nextPosX, nextPosY + 1);

		if (chBelowNext == Board::EMPTY)
			return false;
	}

	char nextCh = board.getBoardChar(nextPosX, nextPosY);

	if (nextCh == SMALL_GHOST_ICON) // Case: small ghost bouncing off another small ghost
		return false;

	return Enemy::isValidToMove();
}

void SmallGhost::move() {
	calculatePrevPos();
	eraseFromBoard();
	eraseFromConsole();

	setDirection();

	bool canGhostMove = isValidToMove(); // Checks validation of movement first

	if (!canGhostMove) {
		changeGhostDirection();
		canGhostMove = isValidToMove();

		if (!canGhostMove) { // Case: ghost is stuck
			currDirX = 0; // Stuck, no movement
		}
	}

	executeMove(canGhostMove);
	drawToBoard();
	drawToConsole();
}

void SmallGhost::setDirection() {
	unsigned num = getRandomNumber() % 100;

	if (currDirX == 0) { // Case: ghost was detected stuck last frame
		currDirX = 1;
		return;
	}
	
	if (96 <= num && num <= 100) // Tiny ghost, with a probability of 0.05, changes direction, else keep momentum
		changeGhostDirection();
}