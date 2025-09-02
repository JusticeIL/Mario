#pragma once
#include "Asset.h"

class Item : public Asset {

public:
	Item(int x, int y, char symbol, bool& isColor) : Asset(x, y, symbol, isColor) {}
	virtual ~Item() = default;
};

