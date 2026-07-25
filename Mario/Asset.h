#pragma once
#include "Board.h"

class Asset {

protected:
	int prevPosX;
	int x;
	int currDirX;
	int prevPosY;
	int y;
	int currDirY;
	char prevCh;
	char icon;
	const bool& isColor;
	char* color;

	// Board
	Board& board;

	Asset(int x, int y, char symbol, bool& isColor, Board& b) : prevPosX(-1), x(x), currDirX(0), prevPosY(-1), y(y), currDirY(0), prevCh(Board::EMPTY),
		icon(symbol), isColor(isColor), color(nullptr),	board(b) {}

public:
	virtual ~Asset() {
		eraseFromBoard();
		eraseFromConsole();
	}

	void calculatePrevPos();
	virtual void drawToConsole() const = 0;
	virtual void drawToBoard() = 0;
	virtual void eraseFromConsole() const = 0;
	void eraseFromBoard() const { board.setBoardChar(prevPosX, prevPosY, prevCh); }
};