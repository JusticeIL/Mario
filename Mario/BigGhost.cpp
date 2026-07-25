#include "BigGhost.h"

#include "Tiles.h"

void BigGhost::setDirection() { // "Sophisticated algorithm that tries to hunt Mario"
	char ChBelow = board.isWithinBounds(x, y + 1) ? board.getBoardChar(x, y + 1) : '\0';

	if (y != marioY)
	{
		bool canClimb = tryClimb();

		if (!canClimb)
		{
			if (x < marioX)
				currDirX = 1;
			else if (x == marioX)
				currDirX = 0;
			else // Case: Mario is behind the ghost
				currDirX = -1;
		}
	}

	else // Case: Ghost is chasing mario on the current floor
	{
		if (x < marioX && (!onLadder || Tiles::isTile(ChBelow) || ChBelow == Board::WALL))
			currDirX = 1;
		else if (x > marioX && (!onLadder || Tiles::isTile(ChBelow) || ChBelow == Board::WALL))
			currDirX = -1;
	}
}

bool BigGhost::tryClimb() {
	updateState();

	if (y > marioY && onLadder) { // Case: Mario is above the ghost
		currDirY = -1;
		return true;
	}
	if (y < marioY && onLadder) { // Case: Mario is below the ghost
		currDirY = 1;
		return true;
	}

	return false; // Case: couldn't climb a ladder
}

void BigGhost::updateState() {
	char chBelowBelow = board.isWithinBounds(x, y + 2) ? board.getBoardChar(x, y + 2) : '\0'; // Char below^2 Ghost
	char chBelow = board.isWithinBounds(x, y + 1) ? board.getBoardChar(x, y + 1) : '\0';  // Char below Ghost
	char chAbove = board.isWithinBounds(x, y - 1) ? board.getBoardChar(x, y - 1) : '\0';  // Char above Ghost

	if (chBelow == 'H' || (chAbove == 'H' && y < marioY))  // Case: on a ladder, or should climb up
		onLadder = true;
	else if ((chBelow == '=' || chBelow == '>' || chBelow == '<' || chBelow == Board::WALL) && prevCh != 'H') // Case: Ghost is on a valid floor
		onLadder = false;
	else if (prevCh == 'H' && y > marioY) // Case: Climbing one-step ladder
		onLadder = true;
	else if (chBelow != 'H' && y <= marioY) // Case: Done descending on the current ladder
		onLadder = false;

	if ((prevCh == '<' || prevCh == '>' || prevCh == '=' || prevCh == Board::WALL) && (chAbove == Board::EMPTY || chBelow == 'H')) // Case: Ghost is climbing and its prevCh is still a floor
		onLadder = true;

	if (prevCh == 'H' && chBelow != 'H' && chBelowBelow != 'H' && y < marioY) // Case: Ghost is in the bottom of the ladder but should not climb
		onLadder = false;
}

void BigGhost::move() {
	calculatePrevPos();
	eraseFromBoard();
	eraseFromConsole();
	setDirection();

	char chBelow = board.isWithinBounds(x, y + 1) ? board.getBoardChar(x, y + 1) : '\0';  // Char below Ghost
	char chBelowNext = board.isWithinBounds(x + currDirX, y + 1) ? board.getBoardChar(x + currDirX, y + 1) : '\0';

	int nextPosX = x + currDirX;
	int nextPosY = y + currDirY;
	char nextCh = board.isWithinBounds(nextPosX, nextPosY) ? board.getBoardChar(nextPosX, nextPosY) : '\0';

	bool canGhostMove = isValidToMove();
	if (chBelowNext == Board::EMPTY || nextCh == Board::WALL || !canGhostMove)
		currDirX = currDirY = 0;

	if (y < marioY && board.getBoardChar(x, y + 2) == 'H') {
		currDirY = 1;
		nextPosX = x + currDirX;
		nextPosY = y + currDirY;
		nextCh = board.isWithinBounds(nextPosX, nextPosY) ? board.getBoardChar(nextPosX, nextPosY) : '\0';
		prevCh = nextCh;
		prevPosX = nextPosX;
		prevPosY = nextPosY;
		onLadder = true;
		canGhostMove = true;
	}
	else if (y > marioY && onLadder) {
		currDirY = -1;
		nextPosX = x + currDirX;
		nextPosY = y + currDirY;
		nextCh = board.isWithinBounds(nextPosX, nextPosY) ? board.getBoardChar(nextPosX, nextPosY) : '\0';
		prevCh = nextCh;
		prevPosX = nextPosX;
		prevPosY = nextPosY;
		onLadder = true;
		canGhostMove = true;
	}

	// Check if the big ghost is stuck and can't move
	if ((board.getBoardChar(x + 1, y + 1) == Board::EMPTY || board.getBoardChar(x + 1, y) == Board::WALL)
		&& (board.getBoardChar(x - 1, y + 1) == Board::EMPTY || board.getBoardChar(x - 1, y) == Board::WALL)
		&& chBelow == Board::EMPTY) // Case: ghost stuck
		currDirX = currDirY = 0;

	executeMove(canGhostMove);
	drawToBoard();
	drawToConsole();
}