#include "Mario.h"
#include "Board.h"
#include "Tiles.h"
#include "Barrel.h"
#include "Pauline.h"
#include "Hammer.h"
#include "SmallGhost.h"
#include "BigGhost.h"
#include "DonkeyKong.h"

void Mario::updateState() {
	char chBelow = board.getBoardChar(x, y + 1);
	char chAbove = board.getBoardChar(x, y - 1);
	char nextCh = board.getBoardChar(x + currDirX, y + currDirY);

	if (!isOnGround && jumping && jumpCounter < MAX_JUMP_HEIGHT) { // Case: continue jumping
		falling = false;
		++jumpCounter;
	}
	else if (!isOnGround && jumping && jumpCounter >= MAX_JUMP_HEIGHT) {
		jumping = false;
		falling = true;
	}
	else if (chBelow == Board::EMPTY) { // Case: Mario is falling or jumping
		isOnGround = false;
		onLadder = false;
	}
	else if (Tiles::isTile(chBelow) && prevCh != Board::LADDER) { // Case: Mario is on the ground
		isOnGround = true;
		onLadder = false;
		falling = false;
	}
	else if (prevCh == Board::LADDER && isOnGround && pressedkey == Key::Up) { // Case: climbing a ladder from a floor
		onLadder = true;
		falling = false;
	}
	else if (onLadder && chBelow != Board::LADDER) { // Case: Mario is on the ladder and pressed DOWN key to go down, he should be able to move to a valid ground
		isOnGround = true;
		falling = false;
	}
	else if ((Tiles::isTile(prevCh) || prevCh == Board::WALL) && currDirY == -1 && (chAbove == Board::EMPTY || chAbove == Board::LADDER)) { // Case: Prevent mario from jumping while reaching to a floor
		jumping = false;
		onLadder = true;
	}
	else if (chBelow == Board::LADDER || chAbove == Board::LADDER) { // Case: Mario is on the ladder
		isOnGround = false;
		onLadder = true;
		falling = false;
	}
	else if (isOnGround && pressedkey == Key::Up && !jumping && prevCh != Board::LADDER) { // Case: start jumping
		onLadder = false;
		falling = false;
		jumping = true;
		++jumpCounter;
	}

	if (prevCh == Hammer::HAMMER_ICON) {
		withHammer = true;
	}

}

bool Mario::isValid() {
	updateState();

	int nextPosX = x + currDirX; // Direction in x axis
	int nextPosY = y + currDirY; // Direction in y axis

	if (!board.isWithinBounds(nextPosX, nextPosY)) {
		return false;
	}

	char nextCh = board.getBoardChar(nextPosX, nextPosY); //Update next char
	char chBelow = board.getBoardChar(x, y + 1); //Update char below

	// Handle movement based on the next char

	if (Tiles::isTile(nextCh)) { // Tiles
		if (onLadder && pressedkey == Key::Up) {
			return true;
		}
		else if (isOnGround && pressedkey == Key::Down && board.getBoardChar(x, y + 2) == Board::LADDER) { // Case: go down the ladder through a wall
			return true;
		}
		else { // Case: Mario cannot walk into tiles
			return false;
		}
	}

	switch (nextCh) {
	case Board::EMPTY: // Empty space
		if (isOnGround || onLadder) { // Case: Mario can move to empty space if on the ground or if on ladder
			return true;
		}
		else { // Case: Mario can't move if not on the ground or on the ladder
			return false;
		}
	case Board::LADDER:
		return true;
	case Board::WALL:
		return false; // Mario can't move through walls
	case Barrel::BARREL_ICON:
		return true;
	case Pauline::PAULINE_ICON: 
		winCon = true;
		return true;
	case Hammer::HAMMER_ICON:
		return true;
	case SmallGhost::SMALL_GHOST_ICON:
		return true;
	case BigGhost::BIG_GHOST_ICON:
		return true;
	case DonkeyKong::DONKEY_KONG_ICON:
		return true;
	default: // Invalid chars
		return false;
	}
}

void Mario::move() {
	bool canMove = isValid();
	
	if (canMove) {
		int nextPosX = x + currDirX; // Direction in x axis
		int nextPosY = y + currDirY; // Direction in y axis
		char nextCh = board.getBoardChar(nextPosX, nextPosY);
		if (jumping) {
			currDirY = -1;
		}
		else if (isOnGround && fallCounter >= MAX_FALL_COUNTER) {
			dead = true;
			resetDir();
		}
		else if (falling) {
			currDirX = 0;
			currDirY = 1;
		}
	}
	else { // Mario cannot move
		resetDir();
	}

}