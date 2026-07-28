#include "Pauline.h"

void Pauline::updateWinCondition(int marioX, int marioY) {
	if (this->x == marioX && this->y == marioY) 
		hasMarioArrived = true;
}