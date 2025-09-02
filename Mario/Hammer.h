#pragma once
#include "Item.h"

class Hammer : public Item {

public:
	Hammer(int x, int y, bool& isColor) : Item(x, y, HAMMER_ICON, isColor) {}

	static constexpr char HAMMER_ICON = 'p';
};