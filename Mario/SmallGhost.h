#pragma once
#include "Ghost.h"

class SmallGhost : public Ghost {

	static constexpr char symbol = 'x';

public:
	SmallGhost(int x, int y, bool& isColor) : Ghost(x, y, symbol, isColor) {}
};

