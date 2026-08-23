#pragma once
#include "Enemy.h"

class MovingEnemy : public Enemy {

public:
	MovingEnemy(int x, int y, char symbol, const char* color, Board& b, bool& isColor) : Enemy(x, y, symbol, color, b, isColor) {}

	void executeMove(bool canMove);
	virtual void move() = 0;
	void killed() { isDead = true; }
};