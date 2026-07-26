#pragma once
#include "Enemy.h"

class MovingEnemy : public Enemy {

public:
	MovingEnemy(int x, int y, char symbol, bool& isColor, Board& b) : Enemy(x, y, symbol, isColor, b) {}

	void executeMove(bool canMove);
	virtual void move() = 0;
};