#pragma once
#include "Enemy.h"

class Barrel : public Enemy {

	static constexpr char symbol = 'O';
	Barrel(int x, int y, bool& isColor) : Enemy(x, y, symbol, isColor) {}

	void move() override;
	void erase() override;

	friend class BarrelFactory;
};