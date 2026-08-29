#pragma once
#include <windows.h>
#include "Colors.h"
#include "GameManager.h"

class Legend {
	// Constants
	static constexpr const char* LEGEND_COLOR = BLUE;
	static constexpr int MAX_LEGEND_WIDTH = 12;
	static constexpr const char* SCORE_STR = "Score: ";
	static constexpr const char* LIVES_STR = "Lives: ";

	int startX;
	int startY;
	int endX;
	int endY;
	const unsigned int& marioLives;
	const unsigned int& score;
	const bool& isColor;

	void wipeOldLegendArea() const;

public:
	Legend(int x, int y, const unsigned int& marioLives, const unsigned int& score, const bool& isColor)
		: startX(x), startY(y), marioLives(marioLives), score(score), isColor(isColor) { // Constructor
		endY = startY + 2;
		endX = min(startX + 20, GameManager::MAX_X - 2);
	}

	void drawToConsole() const;
	void flashYellow() const;
	bool isScoreCoordinate(int checkX, int checkY) const;
};