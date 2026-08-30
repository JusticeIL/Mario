#include <iostream>
#include "Board.h"
#include "DonkeyKong.h"
#include "Pauline.h"
#include "Hammer.h"
#include "HelperFunc.h"
#include "Legend.h"
#include "Tiles.h"
#include "Barrel.h"
#include "ExtraLife.h"

using std::string;

// This function restores the board to the original level's layout using copy assignment, erasing everything the entities drew on it
void Board::reset() {
    if (!currentOriginalLevel)
        throw std::runtime_error("Board::reset called before setLevel");

    gameBoard = currentOriginalLevel->getOriginalLevel();
}

// This function prints the current state of the board
void Board::print(bool isColor, const Legend& legend) const {
    gotoxy(0, 0);
    size_t currentIndex = 0;
    size_t totalLines = gameBoard.size();

    for (const string& line : gameBoard) {
	    if (isColor)
            for (char ch : line)
                printCharWithColor(ch);
        else
            std::cout << line;

        if (++currentIndex < totalLines)
            std::cout << std::endl;
    }

    legend.drawToConsole();
}

// This function receives a char, and prints it to the console in the color that matches its type
void Board::printCharWithColor(char ch) {
    if (ch == LADDER)
        std::cout << LADDER_COLOR << ch << RESET;
    else if (Tiles::isTile(ch))
        std::cout << TILES_COLOR << ch << RESET;
    else if (ch == WALL)
        std::cout << WALL_COLOR << ch << RESET;
    else if (ch == Barrel::BARREL_ICON)
        std::cout << Barrel::BARREL_COLOR << ch << RESET;
    else if (ch == DonkeyKong::DONKEY_KONG_ICON)
        std::cout << DonkeyKong::DONKEYKONG_COLOR << ch << RESET;
    else if (ch == Pauline::PAULINE_ICON)
        std::cout << Pauline::PAULINE_COLOR << ch << RESET;
    else if (ch == Hammer::HAMMER_ICON)
        std::cout << Hammer::HAMMER_COLOR << ch << RESET;
    else if (ch == ExtraLife::EXTRA_LIFE_ICON)
        std::cout << ExtraLife::EXTRA_LIFE_COLOR << ch << RESET;
    else
        std::cout << ch;
}

// This function receives a position, and returns the char the board holds there, or a wall for a position outside the board and a floor for one below its bottom
char Board::getBoardChar(int x, int y) const {
	if (!isWithinBounds(x, y)) {
		if (y >= Board::MAX_Y)
			return Tiles::OUT_OF_BOUNDS_FALLBACK_FLOOR;

        return WALL;
	}

    return gameBoard[y][x];
}

// This function receives a position and a char, and writes the char to the board at that position if it is inside the board
void Board::setBoardChar(int x, int y, char ch) {
	if (isWithinBounds(x, y))
        gameBoard[y][x] = ch;
}

// This function receives the new level, and loads its layout as the current board
void Board::setLevel(const Level* newLevel) {
    currentOriginalLevel = newLevel;
    gameBoard = newLevel->getOriginalLevel();
}