#include "Mario.h"
#include "Board.h"
#include "Tiles.h"
#include "Barrel.h"
#include "Pauline.h"
#include "Hammer.h"
#include "SmallGhost.h"
#include "BigGhost.h"
#include "DonkeyKong.h"

void Mario::setPressedKey(char key) {
	key = tolower(key);
	switch (static_cast<Key>(key)) {
	case Key::Up:
		pressedkey = Key::Up;
		break;
	case Key::Left:
		pressedkey = Key::Left;
		break;
	case Key::Down:
		pressedkey = Key::Down;
		break;
	case Key::Right:
		pressedkey = Key::Right;
		break;
	case Key::Stay:
		pressedkey = Key::Stay;
		break;
	default:
		pressedkey = Key::Stay;
		break;
	}
}

void Mario::updateState() {
	char chBelow = board.getBoardChar(x, y + 1);
	char chAbove = board.getBoardChar(x, y - 1);
	char nextCh = board.getBoardChar(x + currDirX, y + currDirY);

	if (!isOnGround && jumping && jumpCounter < MAX_JUMP_HEIGHT) { // Case: continue jumping
		falling = false;
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
	}

	if (prevCh == Hammer::HAMMER_ICON) {
		withHammer = true;
	}

}

bool Mario::isValidToMove() {
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
	eraseFromBoard();
	eraseFromConsole();

	bool canMove = isValidToMove();
	
	if (canMove) {
		int nextPosX = x + currDirX; // Direction in x axis
		int nextPosY = y + currDirY; // Direction in y axis
		char nextCh = board.getBoardChar(nextPosX, nextPosY);
		if (jumping) {
			jump();
		}
		else if (isOnGround && fallCounter >= MAX_FALL_COUNTER) {
			isDead = true;
			resetDir();
		}
		else if (falling) {
			fall();
		}
		else { // Case: Mario can walk normally
			x = nextPosX;
			y = nextPosY;
		}
	}
	else { // Mario cannot move
		resetDir();
	}

	drawToBoard();
	drawToConsole();
}

void Mario::fall() {
	// Update direction to fall
	currDirY = 1;

	int nextPosX = x + currDirX; // Direction in x axis
	int nextPosY = y + currDirY; // Direction in y axis

	if (board.isWithinBounds(nextPosX, nextPosY)) {
		char chBelow = board.getBoardChar(nextPosX, nextPosY); //Get char below mario

		if (chBelow == Board::EMPTY) { // On air
			y += currDirY;
			x += currDirX;
			fallCounter++;
		}
		else if (Tiles::isTile(chBelow) || chBelow == Board::WALL) { // Going to hit ground after falling
			isOnGround = true;
			currDirY = 0;
			fallCounter = 0;
			return;
		}
		else {
			resetDir();
			fallCounter = 0;
			return;
		}
	}
	else { // Case: hitting the buttom of the board
		isOnGround = true;
		currDirY = 0;
	}
}

void Mario::jump() {

	currDirY = -1;

	if (jumpCounter < MAX_JUMP_HEIGHT)
	{
		int nextPosX = x + currDirX;
		int nextPosY = y + currDirY;

		if (board.isWithinBounds(nextPosX, nextPosY)) {
			char nextCh = board.getBoardChar(nextPosX, nextPosY);
			if (nextCh == Board::EMPTY || nextCh == Board::LADDER) { // The condition might be redundant
				x = nextPosX;
				y = nextPosY;
				++jumpCounter;
			}
			else if (Tiles::isTile(nextCh) || nextCh == Board::WALL) // Case: Mario bump his head against the ceiling
			{	// TODO: This if might be redundant
				currDirY = 0;
				jumpCounter = MAX_JUMP_HEIGHT; // Stop jumping
			}
			else {
				jumpCounter = MAX_JUMP_HEIGHT;
			}
		}
		else {
			jumping = false;
			jumpCounter = MAX_JUMP_HEIGHT;
		}
	}
}

void Mario::useHammer() {
	hammer->use(x, y, currDirX, currDirY, &hammerUsed);

	if (hammerUsed) {
		pressedkey = Key::Init; // Reset pressed key after using the hammer
	}

	hammerUsed = false;
}

void Mario::resetHammer() {
	hammer->reset();
	withHammer = false;
}

void Mario::reset() {
	// Reset position
	resetDir();
	x = startPosX;
	y = startPosY;
	prevCh = Board::EMPTY;

	// Reset input
	pressedkey = Key::Init;

	// Hammer state
	withHammer = false;
	hammerUsed = false;
	hammer->reset();

	// Jump / fall / ladder state
	jumpCounter = 0;
	fallCounter = 0;
	isOnGround = true;
	jumping = false;
	onLadder = false;
	falling = false;

	// Game state
	isDead = false;
}