#pragma once
#include "Board.h"

class Asset {

protected:
	int x;
	int currDirX;
	int y;
	int currDirY;
	char prevCh;
	char icon;
	const bool& isColor;
	char* color;

	// Board
	Board& board;

	Asset(int x, int y, char symbol, bool& isColor, Board& b) : x(x), y(y), icon(symbol), isColor(isColor), prevCh(Board::EMPTY), currDirX(0), currDirY(0),
		board(b) {}

public:
	virtual ~Asset() {
		eraseFromBoard();
		eraseFromConsole();
	}

	virtual void drawToConsole() const = 0;
	virtual void drawToBoard() = 0;
	virtual void eraseFromConsole() const = 0;
	virtual void eraseFromBoard() = 0;
};