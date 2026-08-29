#include "ExtraLife.h"

// This function receives Mario's position, and returns true if he reached the extra life, marking it as collected, and false otherwise
bool ExtraLife::checkIfMarioPickedUpLife(int marioX, int marioY) {
	if (this->x == marioX && this->y == marioY) {
		setCollected();
		return true;
	}

	return false;
}