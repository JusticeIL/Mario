#pragma once
#include "Colors.h"
#include "Ghost.h"

class BigGhost : public Ghost {

	static constexpr const char* SMALL_GHOST_COLOR = RED;
	int& marioX;
	int& marioY;
	bool onLadder;

public:
	BigGhost(int x, int y, Board& b, bool& isColor, int& marioX, int& marioY) : Ghost(x, y, BIG_GHOST_ICON, b, isColor),
		marioX(marioX), marioY(marioY), onLadder(false) {}

	static constexpr char BIG_GHOST_ICON = 'X';

	// Ghost Actions
	void setDirection();
	bool tryClimb();
	void updateState();
	void move() override;
	bool isValidToMove() override;
};