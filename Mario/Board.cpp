#include <iostream>
#include "Board.h"
#include "DonkeyKong.h"
#include "Pauline.h"
#include "Hammer.h"
#include "HelperFunc.h"
#include "Tiles.h"

using namespace std;

void Board::reset(unsigned int marioLife) {
	gameBoard = currentOriginalLevel->getOriginalLevel();
    gotoxy(getLevel().getLegendPositionX(), getLevel().getLegendPositionY());
    std::string lifeString = "Lives: " + std::to_string(marioLife);
    for (int i = 0; i < lifeString.size(); i++)
	    setBoardChar(getLevel().getLegendPositionX() + i, getLevel().getLegendPositionY(), lifeString[i]);
}

// This function prints the current state of the board
void Board::print(bool isColor) const {
    gotoxy(0, 0);
    size_t currentIndex = 0;
    size_t totalLines = gameBoard.size();

    for (const string& line : gameBoard) {
	    if (isColor)
            for (char ch : line) {
                if (ch == LADDER)
                    std::cout << LADDER_COLOR << ch << RESET;
                else if (ch == WALL || Tiles::isTile(ch))
                    std::cout << TILES_COLOR << ch << RESET;
                else if (ch == Barrel::BARREL_ICON)
                    std::cout << Barrel::BARREL_COLOR << ch << RESET;
                else if (ch == DonkeyKong::DONKEY_KONG_ICON)
					std::cout << DonkeyKong::DONKEYKONG_COLOR << ch << RESET;
                else if (ch == Pauline::PAULINE_ICON)
					std::cout << Pauline::PAULINE_COLOR << ch << RESET;
                else if (ch == Hammer::HAMMER_ICON)
					std::cout << Hammer::HAMMER_COLOR << ch << RESET;
                else
                    std::cout << ch;
            }
        else
            cout << line;

        if (++currentIndex < totalLines)
            std::cout << endl;
    }
}

char Board::getBoardChar(int x, int y) const {
	if (!isWithinBounds(x, y)) {
		if (y >= GameManager::MAX_Y)
			return Tiles::OUT_OF_BOUNDS_FALLBACK_FLOOR;

        return WALL;
	}

    return gameBoard[y][x];
}

void Board::setBoardChar(int x, int y, char ch) {
	if (isWithinBounds(x, y))
        gameBoard[y][x] = ch;
}

void Board::setLevel(const Level* newLevel) {
    currentOriginalLevel = newLevel;
    gameBoard = newLevel->getOriginalLevel();
}