#pragma once
#include "BarrelFactory.h"
#include "Enemy.h"
#include "Colors.h"

class DonkeyKong : public Enemy {

	inline static constexpr const char* DOKNEYKONG_COLOR = BROWN;
	BarrelFactory barrelFactory;

public:
	DonkeyKong(int x, int y, bool& isColor, Board& board) : Enemy(x, y, DONKEY_KONG_ICON, isColor, board), barrelFactory(x, y, board, isColor) {}

	static constexpr char DONKEY_KONG_ICON = '&';
	bool isValid() override {}
};