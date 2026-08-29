#pragma once
#include "Barrel.h"

// Forward declaration
class Board;

class BarrelFactory {

	bool& isColor;
	int creationPosX;
	int creationPosY;
	Board& board;

public:
	BarrelFactory(int dkx, int dky, Board& b, bool& isColor); // Constructor
	Barrel* spawnBarrel() const { return new Barrel(creationPosX, creationPosY, board, isColor); }
};