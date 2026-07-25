#include "Asset.h"

void Asset::calculatePrevPos() {
	if (x != -1 && y != -1) { // Case: not spawned
		prevCh = board.getBoardChar(x, y);
		prevPosX = x;
		prevPosY = y;
	}
	else { // Case: spawned
		prevPosX = x;
		prevPosY = y;
	}
}