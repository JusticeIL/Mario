#pragma once
#include "MovingEnemy.h"

// Forward declaration
class Board;

class Ghost : public MovingEnemy {

public:
	Ghost(int x, int y, char symbol, bool& isColor, Board& b) : MovingEnemy(x, y, symbol, isColor, b) {}
};