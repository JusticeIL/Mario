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
}