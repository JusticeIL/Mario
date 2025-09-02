#pragma once
#include "Ghost.h"

class BigGhost : public Ghost {

	static constexpr char symbol = 'X';
	int& marioX;
	int& marioY;

public:
	BigGhost(int x, int y, bool& isColor, int& marioX, int& marioY) : Ghost(x, y, symbol, isColor), marioX(marioX), marioY(marioY) {}
};