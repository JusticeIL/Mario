#pragma once
#include "Asset.h"

class Item : public Asset {

protected:
	bool isCollected;

	void drawToConsole() const override;
	void drawToBoard() override { prevCh = board.getBoardChar(x, y); board.setBoardChar(x, y, icon); }
	void eraseFromConsole() const override;
	void eraseFromBoard() override { board.setBoardChar(x, y, prevCh); }

public:
	Item(int x, int y, char symbol, bool& isColor, Board& b) : Asset(x, y, symbol, isColor, b),
		isCollected(false) {}
	virtual ~Item() = default;

	void setCollected() { isCollected = true; }
};

