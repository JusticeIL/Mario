#pragma once
#include "Item.h"

class Hammer : public Item {

	static constexpr char symbol = 'p';

public:
	Hammer(int x, int y, bool& isColor) : Item(x, y, symbol, isColor) {}
};