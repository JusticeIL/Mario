#pragma once
#include "Character.h"

class Pauline : public Character {

	static constexpr char symbol = '$';

public:
	Pauline(int x, int y, bool& isColor) : Character(x, y, symbol, isColor) {}
};