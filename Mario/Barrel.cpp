#include <iostream>
#include <windows.h>
#include "Barrel.h"
#include "BigGhost.h"
#include "Hammer.h"
#include "HelperFunc.h"
#include "Mario.h"
#include "SmallGhost.h"
#include "Tiles.h"

// This function saves the characters around the barrel's explosion area into a temporary array for later restoration
void Barrel::savePreviousCharsExplosion() {
	for (int dy = -2; dy <= 0; dy++) {
		for (int dx = -2; dx <= 2; dx++) {
			if (dx == 0 && dy == 0) {
				continue; // Skip the barrel's position
			}
			if (board.isWithinBounds(x + dx, y + dy)) { // Ensure board limits
				char boardChar = board.getBoardChar(x + dx, y + dy);

				if (boardChar == '@') {
					isEnemyHitMario = true;
				}

				prevChars[dy + 2][dx + 2] = boardChar;
			}
		}
	}
}

// This function creates the first radius of the explosion effect around the barrel and updates the board with explosion characters
void Barrel::create_first_radius_exp() const {
	for (int dy = -2; dy <= 0; dy++) {
		for (int dx = -2; dx <= 2; dx++) {

			if (dx == 0 && dy == 0) { // Skip the barrel's position
				continue;
			}

			if (board.isWithinBounds(x + dx, y + dy)) // Ensure board limits
			{
				gotoxy(x + dx, y + dy);

				if (dx == 0 && dy == -1) {
					board.setBoardChar(x + dx, y + dy, '|');
					std::cout << '|'; // Vertical explosion
				}
				else if (dy == 0 && std::abs(dx) == 1) {
					board.setBoardChar(x + dx, y + dy, '-');
					std::cout << '-'; // Horizontal explosion
				}
				else if ((dx == 1 && dy == -1)) {
					board.setBoardChar(x + dx, y + dy, '/');
					std::cout << '/'; // Diagonal explosion
				}
				else if (dx == -1 && dy == -1) {
					board.setBoardChar(x + dx, y + dy, '\\');
					std::cout << '\\'; // Diagonal explosion
				}
			}
		}
	}

	Sleep(EXPLOSION_DELAY_MS); // Delay
}

// This function removes the first radius of the explosion effect by erasing explosion characters from the console
void Barrel::delete_first_radius_exp() const {
	for (int dy = -1; dy <= 0; dy++)
		for (int dx = -1; dx <= 1; dx++) {
			if (dx == 0 && dy == 0) // Skip the barrel's position
				continue; 
			
			if (board.isWithinBounds(x + dx, y + dy)) // Ensure board limits
			{
				gotoxy(x + dx, y + dy);
				std::cout << Board::EMPTY; // Erase explosion character
			}
		}
}

// This function creates the second radius of the explosion effect around the barrel and updates the board with explosion characters
void Barrel::create_second_radius_exp() const {
	for (int dy = -2; dy <= 0; dy++) 
		for (int dx = -2; dx <= 2; dx++) {
			if ((std::abs(dx) <= 1 && std::abs(dy) <= 1)) {
				continue; // Skip inner explosion and barrel's position
			}
			if (std::abs(dx) == 2 || std::abs(dy) == 2)
				if (board.isWithinBounds(x + dx, y + dy)) { // Ensure board limits
					gotoxy(x + dx, y + dy);
					if (dx == 0 && dy == -2) {
						board.setBoardChar(x + dx, y + dy, '|');
						std::cout << '|'; // Vertical expansion
					}
					else if (dy == 0 && std::abs(dx) == 2) {
						board.setBoardChar(x + dx, y + dy, '-');
						std::cout << '-'; // Horizontal expansion
					}
					else if (dx > 0 && dy < 0 && !(dx == 1 && dy == -1)) {
						board.setBoardChar(x + dx, y + dy, '/');
						std::cout << '/'; // Diagonal expansion
					}
					else if (dx < 0 && dy < 0 && !(dx == -1 && dy == -1)) {
						board.setBoardChar(x + dx, y + dy, '\\');
						std::cout << '\\'; // Diagonal expansion
					}
				}
		}
	Sleep(EXPLOSION_DELAY_MS); // Delay
}

// This function restores the characters saved before the explosion effect in the specified area of the board and on screen
void Barrel::restorePrevChars() const {
	for (int dy = -2; dy <= 0; dy++) 
		for (int dx = -2; dx <= 2; dx++) {
			if (dx == 0 && dy == 0) // Skip the barrel's position
				continue;

			// Calculate the indices within the bounds of the prevChars array
			int indexY = dy + 2;
			int indexX = dx + 2;

			// Ensure indexes of prevChars are inside board bounds
			if (0 <= indexY && indexY < 3 && 0 <= indexX && indexX < 5)
				if (board.isWithinBounds(x + dx, y + dy)) {// Ensure board limits
					gotoxy(x + dx, y + dy);
					board.setBoardChar(x + dx, y + dy, prevChars[indexY][indexX]);
					std::cout << prevChars[indexY][indexX];
				}
		}
}

void Barrel::move() {
	calculatePrevPos();
	eraseFromBoard();
	eraseFromConsole();

	char chBelow;
	if (board.isWithinBounds(x, y + 1))
		chBelow = board.getBoardChar(x, y + 1);
	else
		chBelow = '\0';

	// Is the barrel in the air?
	if (chBelow == Board::EMPTY)
		isOnAir = true; // Fall
	else
		isOnAir = false;

	bool canBarrelMove = isValidToMove();

	if (!canBarrelMove) { // Case: stuck or explode
		currDirX = 0;
		currDirY = 0;

		if (Tiles::isTile(chBelow) || chBelow == Board::WALL || chBelow == '\0') 
			fall();

		fallCounter = 0;
	}
	else if (isOnAir) // Case: fall
		fall();

	else {				// Case: roll
		fallCounter = 0;
		setDirection();
	}

	executeMove(canBarrelMove);
	drawToBoard();
	drawToConsole();
}

void Barrel::setDirection() {
	char chBelow = board.getBoardChar(x, y + 1);

	switch (chBelow) {
	case '<':
		currDirY = 0;
		currDirX = -1;
		lastDir_x = -1; // Update last movement to be left
		break;
	case '=': case Board::WALL: // Update last movement based on lastDir_x
		if (lastDir_x == -1) {
			currDirY = 0;
			currDirX = -1;
		}
		else if (lastDir_x == 1) {
			currDirY = 0;
			currDirX = 1;
		}
		/*Continue last direction*/ break;
	case '>':
		currDirY = 0;
		currDirX = 1;
		lastDir_x = 1; // Update last movement to be right
		break;
	default: // Space
		currDirY = 1;
		currDirX = 0;
		break;
	}
}

void Barrel::fall() {
	if (fallCounter >= 8 && (!isOnAir || y == (Game::MAX_Y - 1))) {
		explode();
		return;
	}

	currDirX = 0;
	currDirY = 1;

	char chBelow = board.getBoardChar(x, y + 1); // Check the char below the barrel to change movement and state of the barrel
	if (Tiles::isTile(chBelow) || chBelow == Board::WALL) 
		setDirection();
	
	else 
		isOnAir = true; // If not on a floor, continue falling

	fallCounter++;
}

// This function triggers the barrel's explosion, manages the explosion effects, restores the affected area, and marks the barrel as exploded
void Barrel::explode() {
	eraseFromBoard(); // Remove char of barrel

	// Define the prevChars array and initialize to ' '
	memset(prevChars, Board::EMPTY, sizeof(prevChars));

	// Explosion FX
	savePreviousCharsExplosion();
	create_first_radius_exp();
	delete_first_radius_exp();
	create_second_radius_exp();
	restorePrevChars();

	isExploded = true; // End of explosion
}