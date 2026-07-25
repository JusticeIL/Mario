#pragma once
#include "Colors.h"
#include "Ghost.h"

class BigGhost : public Ghost {

	static constexpr const char* SMALL_GHOST_COLOR = RED;
	int& marioX;
	int& marioY;
	bool onLadder;

public:
	BigGhost(int x, int y, bool& isColor, int& marioX, int& marioY, Board& b) : Ghost(x, y, BIG_GHOST_ICON, isColor, b),
		marioX(marioX), marioY(marioY), onLadder(false) {}

	static constexpr char BIG_GHOST_ICON = 'X';

	// Ghost Actions
	void setDirection();
	bool tryClimb();
	void updateState();
	void move() override;
};