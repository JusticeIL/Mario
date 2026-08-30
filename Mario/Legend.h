#pragma once
#include <windows.h>
#include "Colors.h"
#include "Board.h"

class Legend {
	// Constants
	static constexpr const char* LEGEND_COLOR = BLUE;
	static constexpr int MAX_LEGEND_WIDTH = 20;
	static constexpr const char* SCORE_STR = "Score: ";
	static constexpr const char* LIVES_STR = "Lives: ";

	// Legend area bounds
	int startX;
	int startY;
	int endX;
	int endY;

	// Displayed values
	const unsigned int& marioLives;
	const unsigned int& score;

	// Game setting
	const bool& isColor;

	// Legend area cleaner
	void wipeOldLegendArea() const;

public:
	Legend(int x, int y, const unsigned int& marioLives, const unsigned int& score, const bool& isColor)
		: startX(x), startY(y), marioLives(marioLives), score(score), isColor(isColor) { // Constructor
		endY = startY + 2;
		endX = min(startX + MAX_LEGEND_WIDTH, Board::MAX_X - 2);
	}

	// Drawing
	void drawToConsole() const;
	void flashYellow() const;

	// Area verification
	bool isScoreCoordinate(int checkX, int checkY) const;
};