#include "Asset.h"
#include "Board.h"
#include "HelperFunc.h"
#include "Tiles.h"
#include "Colors.h"
#include "Barrel.h"
#include "DonkeyKong.h"
#include "Pauline.h"
#include "Hammer.h"

Asset::Asset(int x, int y, char symbol, const char* color, Board& b, bool& isColor) : prevPosX(-1), x(x), currDirX(0), prevPosY(-1), y(y), currDirY(0),
                                                                                      prevCh(Board::EMPTY), icon(symbol), isColor(isColor), color(color), board(b) {} // Constructor

void Asset::calculatePrevPos() {
	prevPosX = x;
	prevPosY = y;
}

void Asset::eraseFromConsole() const {
	gotoxy(prevPosX, prevPosY);

	if (isColor) {
		if (prevCh == Board::LADDER)
			std::cout << Board::LADDER_COLOR << prevCh << RESET;
		else if (Tiles::isTile(prevCh))
			std::cout << Board::TILES_COLOR << prevCh << RESET;
		else if (prevCh == Board::WALL)
			std::cout << Board::WALL_COLOR << prevCh << RESET;
		else if (prevCh == Barrel::BARREL_ICON)
			std::cout << Barrel::BARREL_COLOR << prevCh << RESET;
		else if (prevCh == DonkeyKong::DONKEY_KONG_ICON)
			std::cout << DonkeyKong::DONKEYKONG_COLOR << prevCh << RESET;
		else if (prevCh == Pauline::PAULINE_ICON)
			std::cout << Pauline::PAULINE_COLOR << prevCh << RESET;
		else if (prevCh == Hammer::HAMMER_ICON)
			std::cout << Hammer::HAMMER_COLOR << prevCh << RESET;
		else
			std::cout << prevCh;
	}
	else
		std::cout << prevCh;
}

void Asset::eraseFromBoard() const {
	board.setBoardChar(prevPosX, prevPosY, prevCh);
}