#include "Pauline.h"

// This function receives Mario's position, and marks that he arrived at Pauline if he reached her
void Pauline::updateWinCondition(int marioX, int marioY) {
	if (this->x == marioX && this->y == marioY) 
		hasMarioArrived = true;
}