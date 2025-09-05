#include "Character.h"
#include "HelperFunc.h"
#include <iostream>
#include "Colors.h"

void Character::drawToBoard() {
	if (!isDead) {
		prevCh = board.getBoardChar(x, y);
		board.setBoardChar(x, y, icon);
	}
}

void Character::drawToConsole() const {
	if (!isDead) {
		gotoxy(x, y);
		if (isColor) {
			std::cout << color << icon << RESET;
		}
		else { // Case: no color mode
			std::cout << icon;
		}
	}
}

void Character::eraseFromConsole() const {
		gotoxy(x, y);
		std::cout << prevCh;
}

void Character::eraseFromBoard() {
		board.setBoardChar(x, y, prevCh);
}