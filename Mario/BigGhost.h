#pragma once
#include "Ghost.h"

class BigGhost : public Ghost {

	int& marioX;
	int& marioY;

public:
	BigGhost(int x, int y, bool& isColor, int& marioX, int& marioY) : Ghost(x, y, BIG_GHOST_ICON, isColor), marioX(marioX), marioY(marioY) {}

	static constexpr char BIG_GHOST_ICON = 'X';

	void move() override;
	void erase() override;
};