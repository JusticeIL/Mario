#pragma once
#include "Enemy.h"

class MovingEnemy : public Enemy {

public:
	MovingEnemy(int x, int y, char symbol, Board& b, bool& isColor) : Enemy(x, y, symbol, b, isColor) {}

	void executeMove(bool canMove);
	virtual void move() = 0;
	void killed() { isDead = true; }
};