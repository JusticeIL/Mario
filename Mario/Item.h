#pragma once
#include "Asset.h"

class Item : public Asset {

protected:
	bool isCollected;

	void drawToConsole() const override;
	void drawToBoard() override;

public:
	Item(int x, int y, char symbol, const char* color, Board& b, bool& isColor) : Asset(x, y, symbol, color, b, isColor),
		isCollected(false) {}
	virtual ~Item() = default;

	virtual void setCollected() { isCollected = true; }
};

