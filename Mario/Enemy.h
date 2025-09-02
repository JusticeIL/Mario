#pragma once
#include "Character.h"

class Enemy : public Character {

public:
	Enemy(int x, int y, char symbol, bool& isColor) : Character(x, y, symbol, isColor) {}
	virtual void move() = 0;
	virtual void erase() = 0;

	bool isHitMario(int marioX, int marioY) const { return (x == marioX && y == marioY); }
	
};