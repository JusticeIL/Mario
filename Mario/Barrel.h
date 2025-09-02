#pragma once
#include "Enemy.h"
#include "Colors.h"

class Barrel : public Enemy {

	Barrel(int x, int y, bool& isColor) : Enemy(x, y, BARREL_ICON, isColor) {}

	friend class BarrelFactory;

public:
	static constexpr char BARREL_ICON = 'O';
	void move() override;
};