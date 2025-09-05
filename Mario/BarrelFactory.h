#pragma once
#include <string>
#include "Barrel.h"
#include "Board.h"

class BarrelFactory {

	inline static constexpr const char* BARREL_COLOR = BROWN;
	bool isColor;
	int creationPosX;
	int creationPosY;
	Board& board;

	enum class BarrelDirection : int { Init = 0, Left = -1, Right = 1, Down = -1 };
	BarrelDirection barrelDirection = BarrelDirection::Init;

public:
	BarrelFactory(int dkx, int dky, Board& b, bool& isColor) : board(b), isColor(isColor) {
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
			creationPosX = Game::MIN_X - 1;
			creationPosY = Game::MIN_Y - 1;
		}
	}
	Barrel* spawnBarrel(bool& isColor) { return new Barrel(creationPosX, creationPosY, isColor, board); }
};