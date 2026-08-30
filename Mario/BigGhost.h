#pragma once
#include "Ghost.h"

class BigGhost : public Ghost {
	// References to Mario's position
	int& marioX;
	int& marioY;

	// Data member
	bool onLadder;

	// Ghost sophisticated search
	void searchMario();
	void setDirection();
	bool tryClimb();

	// State updates
	void updateState();

protected:
	// Validation
	bool isValidToMove() override;

public:
	BigGhost(int x, int y, Board& b, bool& isColor, int& marioX, int& marioY) : Ghost(x, y, BIG_GHOST_ICON, b, isColor),
		marioX(marioX), marioY(marioY), onLadder(false) { // Constructor
	}

	// Constant
	static constexpr char BIG_GHOST_ICON = 'X';

	// Ghost Action
	void move() override;
};