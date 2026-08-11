#pragma once
#include <algorithm>
#include <windows.h>
#include "Colors.h"
#include "GameManager.h"

class Legend {
	// Constants
	static constexpr const char* LEGEND_COLOR = BLUE;
	static constexpr int MAX_LEGEND_WIDTH = 12;

	int startX;
	int startY;
	int endX;
	int endY;
	const unsigned int& marioLives;
	const unsigned int& score;
	const bool& isColor;

public:
	Legend(int x, int y, const unsigned int& marioLives, const unsigned int& score, const bool& isColor)
		: startX(x), startY(y), marioLives(marioLives), score(score), isColor(isColor) { // Constructor
		endY = startY + 2;
		endX = min(startX + 20, GameManager::MAX_X - 2);
	}

	void drawToConsole() const;
};