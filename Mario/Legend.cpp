#include "Legend.h"
#include "Board.h"
#include "HelperFunc.h"

void Legend::drawToConsole() const {
	// 1. Wipe the old legend area cleanly
	int lineIndex = startY;
	while (lineIndex <= endY) {
		gotoxy(startX, lineIndex);
		std::cout << std::string(endX - startX + 1, Board::EMPTY); // Clear the line

		++lineIndex;
	}

	// 2. Draw the new data
	gotoxy(startX, startY);
	std::cout << (isColor ? LEGEND_COLOR : "") << "Lives: " << marioLives << (isColor ? RESET : "");

	gotoxy(startX, startY + 1);
	std::cout << (isColor ? LEGEND_COLOR : "") << "Score: " << score << (isColor ? RESET : "");
}