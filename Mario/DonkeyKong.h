#pragma once
#include "BarrelFactory.h"
#include "Enemy.h"
#include "Colors.h"

class DonkeyKong : public Enemy {

	static constexpr const char* DONKEYKONG_COLOR = BROWN;
	BarrelFactory barrelFactory;

public:
	DonkeyKong(int x, int y, bool& isColor, Board& board) : Enemy(x, y, DONKEY_KONG_ICON, isColor, board),
		barrelFactory(x, y, board, isColor) {
		currDirX = 0;
		currDirY = 0;
	}

	static constexpr char DONKEY_KONG_ICON = '&';

	Barrel* createBarrel() const { return barrelFactory.spawnBarrel(); }
};