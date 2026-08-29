#include "Item.h"
#include <iostream>
#include "HelperFunc.h"
#include "Board.h"
#include "Colors.h"

// This function prints the item's icon on the console and does nothing in silent mode
void Item::drawToConsole() const {
	if (board.isSilent())
		return;

	gotoxy(x, y);
	if (isColor)
		std::cout << color << icon << RESET;
	else // Case: no color mode
		std::cout << icon;
}

// This function draws the item's icon on the board and saves the char that was there before it
void Item::drawToBoard() {
	prevCh = board.getBoardChar(x, y);
	board.setBoardChar(x, y, icon);
}