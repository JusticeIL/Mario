#pragma once
#include "Colors.h"
#include "MovingEnemy.h"

// Forward declaration
class Board;

class Ghost : public MovingEnemy {

	// Constants
	static constexpr const char* GHOST_COLOR = RED;

public:
	Ghost(int x, int y, char symbol, Board& b, bool& isColor) : MovingEnemy(x, y, symbol, GHOST_COLOR, b, isColor) {}
};