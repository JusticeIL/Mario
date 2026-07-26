#include "Enemy.h"
#include "Barrel.h"
#include "BigGhost.h"
#include "Mario.h"
#include "SmallGhost.h"
#include "Tiles.h"

bool Enemy::isValidToMove() {
	int nextPosX = x + currDirX;
	int nextPosY = y + currDirY;

	if (board.isWithinBounds(nextPosX, nextPosY))
		return false;

	char nextCh = board.getBoardChar(nextPosX, nextPosY);

	if (Tiles::isTile(nextCh))
		return false;

	// Check movement by checking next character validation
	switch (nextCh) {
	case Board::EMPTY: // Empty
		return true;
	case Board::LADDER: // Ladder
		return true;
	case Mario::MARIO_ICON: case Mario::MARIO_HAMMER_ICON: // Mario
		return true;
	case Hammer::HAMMER_ICON: // Hammer icon
		return true;
	case Barrel::BARREL_ICON:
	case SmallGhost::SMALL_GHOST_ICON:
	case BigGhost::BIG_GHOST_ICON:
		return true; // Allow stepping into enemies; collision handled later
	case Board::WALL: // Wall
		return false;
	case Mario::HAMMER_ICON: // The hammer of mario
		return false;
	default: // Any others
		return false;
	}
}