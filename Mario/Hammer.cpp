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

// This function receives Mario's position, the direction he faces and a flag pointer, starts a swing at the two cells in front of him, and reports through the flag whether the swing actually started
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

// This function advances the swing's state machine by one tick
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

// This function erases both cells of the swing from the board and from the console, skipping cells that were out of bounds
void Hammer::grab() const {
	for (int i = 0; i < 2; ++i) {
		if (hammerPosX[i] == -1)
			continue;

		eraseHammerCharsFromBoard(i); // Erase the hammer from the board
		eraseHammerCharsFromConsole(i); // Erase the hammer from the console
	}
}

// This function receives the radius of the swing cell, and prints the hammer's icon there, and does nothing in silent mode or if the cell is out of bounds
void Hammer::draw(int radius) const {
	if (board.isSilent() || hammerPosX[radius] == -1)
		return;

	gotoxy(hammerPosX[radius], hammerPosY);
	if (isColor)
		std::cout << HAMMER_COLOR << HAMMER_ICON << RESET;
	else
		std::cout << HAMMER_ICON;
}

// This function receives the radius of the swing cell, and restores on the board the char that was there before the swing, or the level's original char if an enemy was standing there
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

// This function receives the radius of the swing cell, and prints back on the console the char that was there before the swing, or the level's original char if an enemy was standing there, and does nothing in silent mode
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