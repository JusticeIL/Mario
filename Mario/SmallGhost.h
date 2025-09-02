#pragma once
#include "Ghost.h"

class SmallGhost : public Ghost {

	

public:
	SmallGhost(int x, int y, bool& isColor) : Ghost(x, y, SMALL_GHOST_ICON, isColor) {}

	static constexpr char SMALL_GHOST_ICON = 'x';

	void move() override;
	void erase() override;
};
