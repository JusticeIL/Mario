#include <iostream>
#include "Character.h"
#include "BigGhost.h"
#include "HelperFunc.h"
#include "Board.h"
#include "Colors.h"
#include "DonkeyKong.h"
#include "Mario.h"
#include "Pauline.h"
#include "Barrel.h"
#include "SmallGhost.h"
#include "Hammer.h"

// This function draws the character's icon on the board and saves the char that was there before it
void Character::drawToBoard() {
	if (!isDead) {
		char currentBoardChar = board.getBoardChar(x, y);

		if (currentBoardChar == Mario::MARIO_ICON || currentBoardChar == Mario::MARIO_HAMMER_ICON ||
			currentBoardChar == Barrel::BARREL_ICON || currentBoardChar == SmallGhost::SMALL_GHOST_ICON || currentBoardChar == BigGhost::BIG_GHOST_ICON ||
			currentBoardChar == Pauline::PAULINE_ICON || currentBoardChar == DonkeyKong::DONKEY_KONG_ICON) {

			char originalChar = board.getLevel().getOriginalLevel()[y][x];

			if (originalChar == Mario::MARIO_ICON || originalChar == Mario::MARIO_HAMMER_ICON ||
				originalChar == Barrel::BARREL_ICON || originalChar == SmallGhost::SMALL_GHOST_ICON || originalChar == BigGhost::BIG_GHOST_ICON ||
				originalChar == Pauline::PAULINE_ICON || originalChar == DonkeyKong::DONKEY_KONG_ICON ||
				originalChar == Hammer::HAMMER_ICON)
				prevCh = Board::EMPTY;
			else
				prevCh = originalChar;
		}
		else
			prevCh = currentBoardChar;

		board.setBoardChar(x, y, icon);
	}
}

// This function prints the character's icon on the console and does nothing in silent mode or if it is dead
void Character::drawToConsole() const {
	if (board.isSilent())
		return;

	if (!isDead) {
		gotoxy(x, y);
		if (isColor)
			std::cout << color << icon << RESET;
		else // Case: no color mode
			std::cout << icon;
	}
}