#pragma once
#include "Asset.h"

class Character : public Asset {

public:
	Character(int x, int y, char symbol, bool& isColor) : Asset(x, y, symbol, isColor) {}
	virtual ~Character() = default;
};

