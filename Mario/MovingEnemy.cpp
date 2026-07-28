#include "MovingEnemy.h"

#include "BigGhost.h"
#include "Mario.h"
#include "SmallGhost.h"
#include "Tiles.h"

void MovingEnemy::executeMove(bool canMove) {
	if (canMove) {
		x += currDirX;
		y += currDirY;
	}

	// TODO: implement updateIfDead() here
}
void MovingEnemy::updateIfDead(int hammerX, int hammerY) {
	int nextPosX = x + currDirX;
	int nextPosY = y + currDirY;
	isDead = (x == hammerX && y == hammerY);
}