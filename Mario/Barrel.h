#pragma once
#include "Enemy.h"

class Barrel : public Enemy {
	
	Barrel(int x, int y, bool& isColor) : Enemy(x, y, BARREL_ICON, isColor) {}

	void move() override;
	void erase() override;

	friend class BarrelFactory;

public:
	static constexpr char BARREL_ICON = 'O';
};