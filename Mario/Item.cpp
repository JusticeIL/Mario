#include "Item.h"
#include "HelperFunc.h"
#include <iostream>
#include "Colors.h"

void Item::drawToConsole() const {

	gotoxy(x, y);
	if (isColor) {
		std::cout << color << icon << RESET;
	}
	else { // Case: no color mode
		std::cout << icon;
	}
}

void Item::eraseFromConsole() const {
	gotoxy(x, y);
	std::cout << prevCh;
}