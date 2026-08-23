#include <iostream>
#include <windows.h>
#include "Hammer.h"
#include "Board.h"
#include "Barrel.h"
#include "SmallGhost.h"
#include "BigGhost.h"
#include "HelperFunc.h"
#include "Colors.h"
#include "Tiles.h"

void Hammer::use(int marioX, int marioY, int xDirection, bool* hammerUsed) {
	*hammerUsed = true;

	// 1. Calculate Hammer position logic based on Mario's direction
	if (xDirection == 0 || state != HammerState::Idle) {
		*hammerUsed = false;
		return;
	}

	hammerPosX[0] = marioX + xDirection;
	hammerPosX[1] = marioX + (xDirection * 2);
	hammerPosY = marioY;

	// 2. Check if hammer initial bounds are valid
	if (!board.isWithinBounds(hammerPosX[0], hammerPosY) || board.getBoardChar(hammerPosX[0], hammerPosY) == Board::WALL) {
		*hammerUsed = false;
		return;
	}

	state = HammerState::HalfDeployed;
}

void Hammer::updateState() {
	switch (state) {
		case HammerState::HalfDeployed:
			prevHammerChars[0] = board.getBoardChar(hammerPosX[0], hammerPosY);
			board.setBoardChar(hammerPosX[0], hammerPosY, HAMMER_ICON);
			draw(0);

			state = HammerState::FullyDeployed;
			break;
		case HammerState::FullyDeployed:
		if (board.isWithinBounds(hammerPosX[1], hammerPosY) && board.getBoardChar(hammerPosX[1], hammerPosY) != Board::WALL) {
			prevHammerChars[1] = board.getBoardChar(hammerPosX[1], hammerPosY);
			board.setBoardChar(hammerPosX[1], hammerPosY, HAMMER_ICON);
			draw(1);
		}
		else
			hammerPosX[1] = -1; // Mark out of bounds for grab()
			
			state = HammerState::RestoreAndIdle;
			break;
	case HammerState::RestoreAndIdle:
		grab();
		state = HammerState::Idle;
		break;
	default:
		break;
	}
}

void Hammer::grab() const {
	for (int i = 0; i < 2; ++i) {
		if (hammerPosX[i] == -1)
			continue;

		eraseHammerCharsFromBoard(i); // Erase the hammer from the board
		eraseHammerCharsFromConsole(i); // Erase the hammer from the console
	}
}

void Hammer::draw(int radius) const {
	if (board.isSilent() || hammerPosX[radius] == -1)
		return;

	gotoxy(hammerPosX[radius], hammerPosY);
	if (isColor)
		std::cout << HAMMER_COLOR << HAMMER_ICON << RESET;
	else
		std::cout << HAMMER_ICON;
}

void Hammer::eraseHammerCharsFromBoard(int radius) const {
	if (prevHammerChars[radius] == Barrel::BARREL_ICON || prevHammerChars[radius] == SmallGhost::SMALL_GHOST_ICON
		|| prevHammerChars[radius] == BigGhost::BIG_GHOST_ICON) {
		char originalChar = board.getLevel().getOriginalLevel()[hammerPosY][hammerPosX[radius]];

		if (originalChar == Board::LADDER || Tiles::isTile(originalChar)) 
			board.setBoardChar(hammerPosX[radius], hammerPosY, originalChar);
		else // Case: could be entity respawn point
			board.setBoardChar(hammerPosX[radius], hammerPosY, Board::EMPTY);
	}
	else
		board.setBoardChar(hammerPosX[radius], hammerPosY, prevHammerChars[radius]);
}

void Hammer::eraseHammerCharsFromConsole(int radius) const {
	if (board.isSilent())
		return;

	gotoxy(hammerPosX[radius], hammerPosY);
	char charToPrint = prevHammerChars[radius];

	// Resolve the original character if we hit an enemy
	if (prevHammerChars[radius] == Barrel::BARREL_ICON || prevHammerChars[radius] == SmallGhost::SMALL_GHOST_ICON
		|| prevHammerChars[radius] == BigGhost::BIG_GHOST_ICON) {
		char originalChar = board.getLevel().getOriginalLevel()[hammerPosY][hammerPosX[radius]];

		if (originalChar == Board::LADDER || Tiles::isTile(originalChar))
			charToPrint = originalChar;
		else // Case: could be entity respawn point
			charToPrint = Board::EMPTY;
	}
	
	if (isColor)
		Board::printCharWithColor(charToPrint);
	else
		std::cout << charToPrint;
}

void Hammer::reset() {
	std::memset(hammerPosX, -1, sizeof(hammerPosX)); // Reset to an invalid position
	std::memset(prevHammerChars, Board::EMPTY, sizeof(prevHammerChars)); // Reset previous chars to empty

	isCollected = false;
	hammerPosY = -1;
	state = HammerState::Idle;
}