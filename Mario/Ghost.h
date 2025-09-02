#pragma once
#include "Enemy.h"

class Ghost : public Enemy {

public:
	Ghost(int x, int y, char symbol, bool& isColor) : Enemy(x, y, symbol, isColor) {}
};

