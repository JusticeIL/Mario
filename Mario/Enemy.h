#pragma once
#include "Character.h"

class Enemy : public Character {

protected:
	bool isEnemyHitMario;

public:
	Enemy(int x, int y, char symbol, bool& isColor, Board& b) : Character(x, y, symbol, isColor, b),
		isEnemyHitMario(false) {}

	bool isHitMario() const { return isEnemyHitMario; }
	void updateIfHitMario(int marioX, int marioY);
	virtual bool isValidToMove();
};