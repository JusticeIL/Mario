#pragma once
#include "Barrel.h"
#include "Colors.h"

// Forward declaration
class Board;

class BarrelFactory {

	static constexpr const char* BARREL_COLOR = BROWN;
	bool& isColor;
	int creationPosX;
	int creationPosY;
	Board& board;

	enum class BarrelDirection : int { Init = 0, Left = -1, Right = 1, Down = -1 };
	BarrelDirection barrelDirection = BarrelDirection::Init;

public:
	BarrelFactory(int dkx, int dky, Board& b, bool& isColor); // Constructor
	Barrel* spawnBarrel() const { return new Barrel(creationPosX, creationPosY, board, isColor); }
};