#include "Asset.h"
#include <iostream>
#include "Board.h"
#include "HelperFunc.h"

Asset::Asset(int x, int y, char symbol, const char* color, Board& b, bool& isColor) : prevPosX(-1), x(x), currDirX(0), prevPosY(-1), y(y), currDirY(0),
                                                                                      prevCh(Board::EMPTY), icon(symbol), isColor(isColor), color(color), board(b) {} // Constructor

void Asset::calculatePrevPos() {
	prevPosX = x;
	prevPosY = y;
}

void Asset::eraseFromConsole() const {
	if (board.isSilent())
		return;

	gotoxy(prevPosX, prevPosY);

	if (isColor)
		Board::printCharWithColor(prevCh);
	else
		std::cout << prevCh;
}

void Asset::eraseFromBoard() const {
	board.setBoardChar(prevPosX, prevPosY, prevCh);
}