#include "Item.h"
#include <iostream>
#include "HelperFunc.h"
#include "Board.h"
#include "Colors.h"

void Item::drawToConsole() const {

	gotoxy(x, y);
	if (isColor)
		std::cout << color << icon << RESET;
	else // Case: no color mode
		std::cout << icon;
}

void Item::drawToBoard() {
	prevCh = board.getBoardChar(x, y);
	board.setBoardChar(x, y, icon);
}