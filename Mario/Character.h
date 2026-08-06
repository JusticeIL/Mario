#pragma once
#include "Asset.h"

// Forward declaration
class Board;

class Character : public Asset {

protected:
	bool isDead;
	void drawToConsole() const override;
	void drawToBoard() override;

public:
	Character(int x, int y, char symbol, Board& b, bool& isColor) : Asset(x, y, symbol, b, isColor),
		isDead(false) {}
	virtual ~Character() = default;

	bool amIDead() const { return isDead; }
};