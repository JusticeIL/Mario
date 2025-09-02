#pragma once
#include "Board.h"

class Asset {

	static constexpr char RESET_TXT[] = "\033[0m";

protected:
	int x;
	int y;
	const char icon;
	const bool& isColor;
	const char color = '\033[0m';

public:
	 Asset(int x, int y, char symbol, bool& isColor) : x(x), y(y), icon(symbol), isColor(isColor) {}
	 void draw() const;
	 void erase() const;
};