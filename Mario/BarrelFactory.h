#pragma once
#include "Barrel.h"

// Forward declaration
class Board;

class BarrelFactory {
	// References to other objects
	Board& board;

	// Reference to the color mode
	bool& isColor;

	// Spawn point
	int creationPosX;
	int creationPosY;

public:
	BarrelFactory(int dkx, int dky, Board& b, bool& isColor); // Constructor

	// Barrel creation
	Barrel spawnBarrel() const { return Barrel(creationPosX, creationPosY, board, isColor); }
};