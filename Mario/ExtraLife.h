#pragma once
#include "Item.h"
#include "Colors.h"

class ExtraLife : public Item {

public:
	ExtraLife(int x, int y, Board& b, bool& isColor) : Item(x, y, EXTRA_LIFE_ICON, EXTRA_LIFE_COLOR, b, isColor) {} // Constructor

	// Constants
	static constexpr char EXTRA_LIFE_ICON = 'T';
	static constexpr const char* EXTRA_LIFE_COLOR = GREEN;

	// Behavior
	bool checkIfMarioPickedUpLife(int marioX, int marioY);

	// Print methods
	void drawToConsole() const override { Item::drawToConsole(); }
	void drawToBoard() override { Item::drawToBoard(); }
};