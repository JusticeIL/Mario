#include "Legend.h"
#include "Board.h"
#include "HelperFunc.h"

void Legend::wipeOldLegendArea() const {
	int lineIndex = startY;
	while (lineIndex <= endY) {
		gotoxy(startX, lineIndex);
		std::cout << std::string(endX - startX + 1, Board::EMPTY); // Clear the line

		++lineIndex;
	}
}

void Legend::drawToConsole() const {
	// 1. Wipe the old legend area cleanly
	wipeOldLegendArea();

	// 2. Draw the new data
	gotoxy(startX, startY);
	std::cout << (isColor ? LEGEND_COLOR : "") << LIVES_STR << marioLives << (isColor ? RESET : "");

	gotoxy(startX, startY + 1);
	std::cout << (isColor ? LEGEND_COLOR : "") << SCORE_STR << score << (isColor ? RESET : "");
}

void Legend::flashYellow() const {
	// 1. Wipe the old legend area cleanly
	wipeOldLegendArea();

	// 2. Draw the new data
	gotoxy(startX, startY);
	std::cout << (isColor ? LEGEND_COLOR : "") << LIVES_STR << marioLives << (isColor ? RESET : "");

	gotoxy(startX, startY + 1);
	std::cout << (isColor ? YELLOW : "") << SCORE_STR << score << (isColor ? RESET : ""); // Temporarily render the text in Yellow
}

bool Legend::isScoreCoordinate(int checkX, int checkY) const {
	if (checkY == startY + 1) {
		unsigned long long scoreTextLength = strlen(SCORE_STR) + std::to_string(score).length();
		return (checkX >= startX && checkX < startX + scoreTextLength);
	}

	return false;
}