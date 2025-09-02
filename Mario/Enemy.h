#pragma once
#include "Character.h"

class Enemy : public Character {

public:
	Enemy(int x, int y, char symbol, bool& isColor) : Character(x, y, symbol, isColor) {}

	bool isHitMario(int marioX, int marioY) const { return (x == marioX && y == marioY); }

	virtual void move() = 0;
	
};