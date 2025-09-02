#pragma once
#include "Character.h"
#include "BarrelFactory.h"

class DonkeyKong : public Character {

	static constexpr char symbol = '&';
	BarrelFactory barrelFactory;

public:
	DonkeyKong(int x, int y, bool& isColor, Board& board) : Character(x, y, symbol, isColor), barrelFactory(x, y, board, isColor) {}
};