#include "BarrelFactory.h"
#include "GameManager.h"
#include "Board.h"

BarrelFactory::BarrelFactory(int dkx, int dky, Board& b, bool& isColor) : isColor(isColor), board(b) { // Constructor
	if (board.isWithinBounds(dkx + 1, dky) && board.getBoardChar(dkx + 1, dky) == Board::EMPTY) { // Case: initialize barrel spawn direction to right (if possible)
		creationPosX = dkx + 1;
		creationPosY = dky;
		barrelDirection = BarrelDirection::Right;
	}
	else if (board.isWithinBounds(dkx - 1, dky) && board.getBoardChar(dkx - 1, dky) == Board::EMPTY) { // Case: initialize barrel spawn direction to left (if possible)
		creationPosX = dkx - 1;
		creationPosY = dky;
		barrelDirection = BarrelDirection::Left;
	}
	else if (board.isWithinBounds(dkx, dky + 1) && board.getBoardChar(dkx, dky + 1) == Board::EMPTY) { // Case: initialize barrel spawn direction to below donkey kong (if possible)
		creationPosX = dkx;
		creationPosY = dky + 1;
		barrelDirection = BarrelDirection::Down;
	}
	else { // Case: No valid spawn point, therefore set invalid spawn point
		creationPosX = GameManager::MIN_X - 1;
		creationPosY = GameManager::MIN_Y - 1;
	}
}