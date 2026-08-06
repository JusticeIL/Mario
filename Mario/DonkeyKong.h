#pragma once
#include "BarrelFactory.h"
#include "Enemy.h"
#include "Colors.h"

class DonkeyKong : public Enemy {

	static constexpr const char* DONKEYKONG_COLOR = BROWN;
	BarrelFactory barrelFactory;

public:
	DonkeyKong(int x, int y, Board& board, bool& isColor) : Enemy(x, y, DONKEY_KONG_ICON, board, isColor),
		barrelFactory(x, y, board, isColor) {
		currDirX = 0;
		currDirY = 0;
	}

	static constexpr char DONKEY_KONG_ICON = '&';

	Barrel* createBarrel() const { return barrelFactory.spawnBarrel(); }
};