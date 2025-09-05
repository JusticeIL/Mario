#pragma once
#include "Item.h"
#include "Colors.h"
#include "Board.h"

class Hammer : public Item {

	inline static constexpr const char* HAMMER_COLOR = BROWN;
	static constexpr int USE_TIME = 300; // Duration in ms to display the hammer
	int hammerPos_x[2];
	int hammerPos_y[2];
	char prevHammerChars[3];

public:
	Hammer(int x, int y, Board& b, bool& isColor) : Item(x, y, HAMMER_ICON, isColor, b) {
		std::memset(hammerPos_x, 0, sizeof(hammerPos_x));
		std::memset(hammerPos_y, 0, sizeof(hammerPos_y));
		std::memset(prevHammerChars, '\0', sizeof(prevHammerChars));
	}

	static constexpr char HAMMER_ICON = 'p';

	void use(int marioX, int marioY, int xDirection, int yDirection, bool* hammerUsed);
	void grab();

	void draw() const;
	void erase() const;
	void reset();
};