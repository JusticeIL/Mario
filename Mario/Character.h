#pragma once
#include "Asset.h"

// Forward declaration
class Board;

class Character : public Asset {

protected:
	// Data member
	bool isDead;

	// Drawing
	void drawToConsole() const override;
	void drawToBoard() override;

public:
	Character(int x, int y, char symbol, const char* color, Board& b, bool& isColor) : Asset(x, y, symbol, color, b, isColor), isDead(false) {} // Constructor
	virtual ~Character() = default; // Destructor

	// Getter
	bool amIDead() const { return isDead; }
};