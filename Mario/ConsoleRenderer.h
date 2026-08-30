#pragma once
#include <windows.h>
#include "Board.h"
#include "HelperFunc.h"
#include "Legend.h"

class ConsoleRenderer {
public:
	// Drawing
	void renderBoard(const Board& board, const Legend& legend, bool isColor) const { board.print(isColor, legend); }
	void clear() const { clearScr(); }

	// Game pace
	void wait(unsigned int refreshRateMs) const { Sleep(refreshRateMs); }
};