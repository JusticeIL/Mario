#pragma once
#include "Character.h"

class DonkeyKong : public Character {

	static constexpr char symbol = '&';

public:
	DonkeyKong(int x, int y, bool& isColor) : Character(x, y, symbol, isColor) {}
};

