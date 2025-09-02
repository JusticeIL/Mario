#pragma once
#include "Ghost.h"

class SmallGhost : public Ghost {

	inline static constexpr const char* SMALL_GHOST_COLOR = RED;

public:
	SmallGhost(int x, int y, bool& isColor) : Ghost(x, y, SMALL_GHOST_ICON, isColor) {}

	static constexpr char SMALL_GHOST_ICON = 'x';

	void move() override;
};
