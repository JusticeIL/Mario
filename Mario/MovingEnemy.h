#pragma once
#include "Enemy.h"

class MovingEnemy : public Enemy {
protected:
	// Enemy movement
	void executeMove(bool canMove);

public:
	MovingEnemy(int x, int y, char symbol, const char* color, Board& b, bool& isColor) : Enemy(x, y, symbol, color, b, isColor) {} // Constructor

	// Enemy actions
	virtual void move() = 0;
	void killed() { isDead = true; }
};