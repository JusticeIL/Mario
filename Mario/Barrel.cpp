#include <iostream>
#include <windows.h>
#include "Barrel.h"
#include "Board.h"
#include "HelperFunc.h"
#include "Mario.h"
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

				if (boardChar == Mario::MARIO_ICON) {
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
				if (!board.isSilent())
					gotoxy(x + dx, y + dy);

				if (dx == 0 && dy == -1) {
					board.setBoardChar(x + dx, y + dy, '|');
					if (!board.isSilent())
						std::cout << '|'; // Vertical explosion
				}
				else if (dy == 0 && std::abs(dx) == 1) {
					board.setBoardChar(x + dx, y + dy, '-');
					if (!board.isSilent())
						std::cout << '-'; // Horizontal explosion
				}
				else if ((dx == 1 && dy == -1)) {
					board.setBoardChar(x + dx, y + dy, '/');
					if (!board.isSilent())
						std::cout << '/'; // Diagonal explosion
				}
				else if (dx == -1 && dy == -1) {
					board.setBoardChar(x + dx, y + dy, '\\');
					if (!board.isSilent())
						std::cout << '\\'; // Diagonal explosion
				}
			}
		}
	}

	if (!board.isSilent())
		Sleep(EXPLOSION_DELAY_MS); // Delay
}

// This function removes the first radius of the explosion effect by erasing explosion characters from the console
void Barrel::delete_first_radius_exp() const {
	if (board.isSilent())
		return;

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

	if (!board.isSilent())
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
				if (board.isWithinBounds(x + dx, y + dy)) { // Ensure board limits
					char charToPrint = prevChars[indexY][indexX];
					board.setBoardChar(x + dx, y + dy, charToPrint);
					
					if (!board.isSilent()) {
						gotoxy(x + dx, y + dy);
						if (isColor)
							Board::printCharWithColor(charToPrint);
						else
							std::cout << charToPrint;
					}
				}
		}
}

void Barrel::move() {
	// Explosion Intercept
	if (explosionState != ExplosionState::NotExploding) {
		processExplosion();
		return;
	}

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
	if (fallCounter >= 8 && (!isOnAir || y == (GameManager::MAX_Y - 1))) {
		startExplode();
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
void Barrel::processExplosion() {
	switch (explosionState) {
	case ExplosionState::Radius1:
		// Remove char of barrel
		eraseFromBoard();
		eraseFromConsole();

		// Define the prevChars array and initialize to ' '
		memset(prevChars, Board::EMPTY, sizeof(prevChars));
		savePreviousCharsExplosion();
		create_first_radius_exp();
		explosionState = ExplosionState::Radius2;
		break;
	case ExplosionState::Radius2:
		delete_first_radius_exp();
		create_second_radius_exp();
		explosionState = ExplosionState::RestoreAndDie;
		break;
	case ExplosionState::RestoreAndDie:
		restorePrevChars();
		isDead = true; // End of explosion
		break;
	default:
		break;
	}
}