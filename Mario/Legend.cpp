#include "Legend.h"
#include <iostream>
#include <cstring>
#include <string>
#include "Board.h"
#include "HelperFunc.h"

// This function clears the whole legend area on the console
void Legend::wipeOldLegendArea() const {
	int lineIndex = startY;
	while (lineIndex <= endY) {
		gotoxy(startX, lineIndex);
		std::cout << std::string(endX - startX + 1, Board::EMPTY); // Clear the line

		++lineIndex;
	}
}

// This function prints Mario's lives and the current score in the legend's area, in the legend's color if color mode is on
void Legend::drawToConsole() const {
	// 1. Wipe the old legend area cleanly
	wipeOldLegendArea();

	// 2. Draw the new data
	gotoxy(startX, startY);
	std::cout << (isColor ? LEGEND_COLOR : "") << LIVES_STR << marioLives << (isColor ? RESET : "");

	gotoxy(startX, startY + 1);
	std::cout << (isColor ? LEGEND_COLOR : "") << SCORE_STR << score << (isColor ? RESET : "");
}

// This function prints the legend, but with the score line in yellow, as the flash effect of the head bump
void Legend::flashYellow() const {
	// 1. Wipe the old legend area cleanly
	wipeOldLegendArea();

	// 2. Draw the new data
	gotoxy(startX, startY);
	std::cout << (isColor ? LEGEND_COLOR : "") << LIVES_STR << marioLives << (isColor ? RESET : "");

	gotoxy(startX, startY + 1);
	std::cout << (isColor ? YELLOW : "") << SCORE_STR << score << (isColor ? RESET : ""); // Temporarily render the text in Yellow
}

// This function receives a position, and returns true if it is on the legend's score text and false otherwise
bool Legend::isScoreCoordinate(int checkX, int checkY) const {
	if (checkY == startY + 1) {
		unsigned long long scoreTextLength = strlen(SCORE_STR) + std::to_string(score).length();
		return (checkX >= startX && checkX < startX + scoreTextLength);
	}

	return false;
}