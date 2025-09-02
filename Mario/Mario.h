#pragma once
#include "Character.h"

class Mario : public Character {

	static constexpr char symbol = '@';

public:
	Mario(int x, int y, bool& isColor) : Character(x, y, symbol, isColor) {}
	int getMarioX() const { return x; }
	int getMarioY() const { return y; }
};