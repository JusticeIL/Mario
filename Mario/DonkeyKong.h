#pragma once
#include "Character.h"
#include "BarrelFactory.h"

class DonkeyKong : public Enemy {

	BarrelFactory barrelFactory;

public:
	DonkeyKong(int x, int y, bool& isColor, Board& board) : Enemy(x, y, DONKEY_KONG_ICON, isColor), barrelFactory(x, y, board, isColor) {}

	static constexpr char DONKEY_KONG_ICON = '&';
};