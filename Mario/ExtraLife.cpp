#include "ExtraLife.h"

bool ExtraLife::checkIfMarioPickedUpLife(int marioX, int marioY) {
	if (this->x == marioX && this->y == marioY) {
		setCollected();
		return true;
	}

	return false;
}