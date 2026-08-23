#pragma once
#include "Board.h"
#include "HelperFunc.h"
#include "Legend.h"

class ConsoleRenderer {
public:
	void renderBoard(const Board& board, const Legend& legend, bool isColor) { board.print(isColor, legend); }
	void wait(unsigned int refreshRateMs) { Sleep(refreshRateMs); }
	void clear() { clearScr(); }
};