#pragma once
#include "Ghost.h"

class BigGhost : public Ghost {

	inline static constexpr const char* SMALL_GHOST_COLOR = RED;
	int& marioX;
	int& marioY;

public:
	BigGhost(int x, int y, bool& isColor, int& marioX, int& marioY) : Ghost(x, y, BIG_GHOST_ICON, isColor), marioX(marioX), marioY(marioY) {}

	static constexpr char BIG_GHOST_ICON = 'X';

	void move() override;
};