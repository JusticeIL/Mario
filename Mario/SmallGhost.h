#pragma once
#include "Ghost.h"

class SmallGhost : public Ghost {

	inline static constexpr const char* SMALL_GHOST_COLOR = RED;

public:
	SmallGhost(int x, int y, bool& isColor, Board& b) : Ghost(x, y, SMALL_GHOST_ICON, isColor, b) {}

	static constexpr char SMALL_GHOST_ICON = 'x';

	void move() override;
	bool isValid() override;
};
