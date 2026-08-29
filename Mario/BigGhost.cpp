#include "BigGhost.h"
#include "Board.h"
#include "Tiles.h"

// This function looks for the closest ladder that leads towards Mario's floor with a clear path to it, and sets the ghost's direction to it, or straight towards Mario if no such ladder was found
void BigGhost::searchMario() {
	if (tryClimb()) // Case: climbing
		return;

	// Where are we heading?
	bool wantUp = (y > marioY);
	bool wantDown = (y < marioY);

	if (y == marioY) { // Case: same floor, but path blocked so we would like to make a detour, looking for a ladder
		wantUp = true;
		wantDown = true;
	}

	int targetLadderX = -1;

	for (int i = GameManager::MIN_X; i < GameManager::MAX_X; ++i) { // Go over current floor level
		char curr = board.getBoardChar(i, y);
		char belowBelow = board.getBoardChar(i, y + 2);
		char above = board.getBoardChar(i, y - 1);

		bool canGoUp = (curr == Board::LADDER);
		bool canGoDown = (belowBelow == Board::LADDER);

		if ((wantUp && canGoUp) || (wantDown && canGoDown)) { // Case: found a potential ladder in the right direction
			bool pathClear = true;
			if (i != x) { // Case: not on ladder anymore
				int step = (i > x) ? 1 : -1;

				for (int j = x + step; j != i; j += step) {
					char theCh = board.getBoardChar(j, y);
					char belowTheCh = board.getBoardChar(j, y + 1);

					if (Tiles::isTile(theCh) || theCh == Board::WALL || (belowTheCh == Board::EMPTY && theCh != Board::LADDER)) {
						pathClear = false;
						break;
					}
				}
			}

			if (pathClear) { // Case: found a ladder with a clear path
				targetLadderX = i;
				break;
			}
		}
	}

	if (targetLadderX != -1) { // Case: found an available ladder, go to it
		if (x < targetLadderX)
			currDirX = 1;
		else if (x > targetLadderX)
			currDirX = -1;
		else // Case: arrived at a ladder
			currDirX = 0;
	}
	else { // Case: fallback - no ladder available, just go towards Mario
		if (x < marioX)
			currDirX = 1;
		else if (x > marioX)
			currDirX = -1;
		else
			currDirX = 0;
	}
}

// This function sets the ghost's direction, going straight towards Mario when they share a floor with a clear path between them, and looking for a ladder to Mario otherwise
void BigGhost::setDirection() { // "Sophisticated algorithm that tries to hunt Mario"
	currDirX = 0;
	currDirY = 0;

	if (y == marioY) { // Case: mario and ghost are on the same floor
		int step = (marioX > x) ? 1 : -1;
		bool clearPath = true;

		for (int i = x + step; i != marioX; i += step) { // Check for clear path horizontally
			char chBelow = board.getBoardChar(i, y + 1);
			char chCurrent = board.getBoardChar(i, y);

			if (Tiles::isTile(chCurrent) || chCurrent == Board::WALL ||
				(chBelow == Board::EMPTY && chCurrent != Board::LADDER)) {
				clearPath = false;
				break;
			}
		}

		if (clearPath) { // Case: clear path to Mario horizontally
			currDirX = step;
			return;
		}
	}

	// Mario is on another floor or path to him is blocked
	searchMario();
}

// This function returns true if the ghost is on a ladder, setting its direction to climb, and false otherwise
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

// This function updates whether the ghost is currently on a ladder
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

// This function performs the big ghost's whole turn: erasing it from its old position, updating its state and direction, moving and drawing it again
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

// This function returns true if the ghost's next position is inside the board and holds a char it can enter, and false otherwise
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