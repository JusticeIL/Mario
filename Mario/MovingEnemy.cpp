#include "MovingEnemy.h"

// This function receives a boolean indicating if the moving enemy can move, and advances the enemy one step if the move is allowed
void MovingEnemy::executeMove(bool canMove) {
	if (canMove) {
		x += currDirX;
		y += currDirY;
	}
}