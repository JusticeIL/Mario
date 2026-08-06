#include <iostream>
#include "Board.h"
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
void Board::print() const {
    gotoxy(0, 0);
    size_t currentIndex = 0;
    size_t totalLines = gameBoard.size();

    for (const string& line : gameBoard) {
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