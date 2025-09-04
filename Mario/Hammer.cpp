#include <iostream>
#include <windows.h>
#include "Hammer.h"
#include "Barrel.h"
#include "SmallGhost.h"
#include "BigGhost.h"
#include "HelperFunc.h"
#include "Colors.h"

void Hammer::use(int marioX, int marioY, int xDirection, int yDirection, bool* hammerUsed) {
	*hammerUsed = true;

	// Hammer position logic depending on Mario's direction
	if (xDirection == -1 || xDirection == 1) {
		hammerPos_x[0] = marioX + xDirection;
		hammerPos_y[0] = marioY + yDirection;
		hammerPos_x[1] = marioX + xDirection * 2;
		hammerPos_y[1] = marioY + yDirection;
	}
	else if (xDirection == 0) { // Case: Mario not moving and therefore cannot use hammer
		*hammerUsed = false;
		return;
	}

	// Check if hammer positions are valid
	char nextCh[3] = {};
	for (int i = 0; i < 2; i++) {
		nextCh[i] = board.getBoardChar(hammerPos_x[i], hammerPos_y[i]);
		if (nextCh[i] == Board::WALL || !board.isWithinBounds(hammerPos_x[i], hammerPos_y[i])) { // Case: hammer hits a wall or goes out of bounds
			*hammerUsed = false;
			return;
		}
	}

	// Update board but do not print
	for (int i = 0; i < 2; i++) {
		board.setBoardChar(hammerPos_x[i], hammerPos_y[i], HAMMER_ICON);
		prevHammerChars[i] = nextCh[i];
	}

	draw();
	Sleep(USE_TIME);
	grab();
}

void Hammer::grab() {
	for (int i = 0; i < 2; i++) {
		if (prevHammerChars[i] != Barrel::BARREL_ICON && prevHammerChars[i] != SmallGhost::SMALL_GHOST_ICON && prevHammerChars[i] != BigGhost::BIG_GHOST_ICON) {
			board.setBoardChar(hammerPos_x[i], hammerPos_y[i], prevHammerChars[i]); // Erase from board
		}
	}
	erase();
}

void Hammer::draw() const { // TODO: when implementing, need to check if hammerUsed is true
		for (int i = 0; i < 2; i++) {
		gotoxy(hammerPos_x[i], hammerPos_y[i]);
		if (isColor) {
			std::cout << HAMMER_COLOR << HAMMER_ICON << RESET;
		}
		else { // Case: no color mode
			std::cout << HAMMER_ICON;
		}
	}
}

void Hammer::erase() const {
	for (int i = 0; i < 2; i++) {
		if (prevHammerChars[i] != Barrel::BARREL_ICON && prevHammerChars[i] != SmallGhost::SMALL_GHOST_ICON && prevHammerChars[i] != BigGhost::BIG_GHOST_ICON) {
			gotoxy(hammerPos_x[i], hammerPos_y[i]);
			std::cout << prevHammerChars[i];
		}
	}
}

void Hammer::reset() {
	std::memset(hammerPos_x, -1, sizeof(hammerPos_x)); // Reset to an invalid position
	std::memset(hammerPos_y, -1, sizeof(hammerPos_y)); // Reset to an invalid position
	std::memset(prevHammerChars, Board::EMPTY, sizeof(prevHammerChars)); // Reset previous chars to empty
}