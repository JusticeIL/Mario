#pragma once
#include "BarrelFactory.h"
#include "Enemy.h"
#include "Colors.h"

class DonkeyKong : public Enemy {

	BarrelFactory barrelFactory;

public:
	DonkeyKong(int x, int y, Board& board, bool& isColor) : Enemy(x, y, DONKEY_KONG_ICON, DONKEYKONG_COLOR, board, isColor),
		barrelFactory(x, y, board, isColor) {
		currDirX = 0;
		currDirY = 0;
	}

	static constexpr char DONKEY_KONG_ICON = '&';
	static constexpr const char* DONKEYKONG_COLOR = BRIGHT_RED;

	Barrel* createBarrel() const { return barrelFactory.spawnBarrel(); }
};