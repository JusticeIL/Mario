#include "SmallGhost.h"

void SmallGhost::move() {
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