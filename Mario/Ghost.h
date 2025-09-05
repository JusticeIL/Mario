#pragma once
#include "Enemy.h"

class Ghost : public Enemy {

public:
	Ghost(int x, int y, char symbol, bool& isColor, Board& b) : Enemy(x, y, symbol, isColor, b) {}
};

