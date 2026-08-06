#pragma once
#include "MovingEnemy.h"

// Forward declaration
class Board;

class Ghost : public MovingEnemy {

public:
	Ghost(int x, int y, char symbol, Board& b, bool& isColor) : MovingEnemy(x, y, symbol, b, isColor) {}
};