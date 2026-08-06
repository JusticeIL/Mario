#include "MovingEnemy.h"

void MovingEnemy::executeMove(bool canMove) {
	if (canMove) {
		x += currDirX;
		y += currDirY;
	}
}