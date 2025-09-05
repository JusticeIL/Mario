#pragma once
#include "Asset.h"

class Character : public Asset {

protected:
	bool isDead;
	void drawToConsole() const override;
	void drawToBoard() override;
	void eraseFromConsole() const override;
	void eraseFromBoard() override;

public:
	Character(int x, int y, char symbol, bool& isColor, Board& b) : Asset(x, y, symbol, isColor, b),
		isDead(false) {}
	virtual ~Character() = default;

	virtual bool isValid() = 0;
};

