#include "Asset.h"
#include "HelperFunc.h"
#include <iostream>

void Asset::draw() const {

	gotoxy(x, y);
	if (isColor) {
		std::cout << color << icon << RESET_TXT;
	}
	else { // Case: no color mode
		std::cout << icon;
	}
}

void Asset::erase() const {
	gotoxy(x, y);
	std::cout << Board::EMPTY;
}