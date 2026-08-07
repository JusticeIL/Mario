#pragma once
#include "Character.h"

// Forward declaration
class Board;

class Enemy : public Character {

protected:
	bool isEnemyHitMario;

public:
	Enemy(int x, int y, char symbol, const char* color, Board& b, bool& isColor) : Character(x, y, symbol, color, b, isColor),
		isEnemyHitMario(false) {}

	bool isHitMario() const { return isEnemyHitMario; }
	void updateIfHitMario(int marioX, int marioY) {	isEnemyHitMario = isEnemyHitMario || (x == marioX && y == marioY);	}
	virtual bool isValidToMove();
};