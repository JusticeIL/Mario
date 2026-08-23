#include "Mario.h"
#include "Board.h"
#include "Tiles.h"
#include "Barrel.h"
#include "Pauline.h"
#include "Hammer.h"
#include "SmallGhost.h"
#include "BigGhost.h"
#include "DonkeyKong.h"
#include "Legend.h"

void Mario::setPressedKey(char key) {
	canJump = true;

	switch (static_cast<Key>(key)) {
	case Key::Up:
		pressedKey = Key::Up;
		break;
	case Key::Left:
		pressedKey = Key::Left;
		break;
	case Key::Down:
		pressedKey = Key::Down;
		break;
	case Key::Right:
		pressedKey = Key::Right;
		break;
	case Key::Stay:
		pressedKey = Key::Stay;
		break;
	case Key::PlayHammer:
		pressedKey = Key::PlayHammer;
		break;
	default:
		pressedKey = Key::Stay;
		break;
	}
}

void Mario::tryMove() {

	if (falling)
		currDirY = 1;
	else if (jumping)
		currDirY = -1;
	else
		currDirY = 0;

	if (hasHammer() && hammer->isSwinging()) {
		currDirX = 0;
		pressedKey = Key::Stay; // Prevents Mario from storing the key and walking after the swing finishes
		return;
	}

	if (pressedKey == Key::Left) {
		currDirX = -1;
		if (!falling && !jumping)
			currDirY = 0;
	}
	else if (pressedKey == Key::Right) {
		currDirX = 1;
		if (!falling && !jumping)
			currDirY = 0;
	}
	else if (pressedKey == Key::Up) {
		currDirX = 0;
		if (!falling && !jumping)
			currDirY = -1;
	}
		
	else if (pressedKey == Key::Down) {
		currDirX = 0;
		if (!falling && !jumping)
			currDirY = 1;
	}
		
	else if (pressedKey == Key::Stay) {
		currDirX = 0;
		if (!falling && !jumping)
			currDirY = 0;
	}

	else { // Case: user can control Y-axis if they are safely on a ladder or ground
		if (pressedKey == Key::Up)
			currDirY = -1;
		else if (pressedKey == Key::Down)
			currDirY = 1;
		else if (pressedKey == Key::Stay)
			currDirY = 0;
	}
}

void Mario::updateState() {
	char chBelow = board.getBoardChar(x, y + 1);
	char chAbove = board.getBoardChar(x, y - 1);
	bool wasOnLadder = onLadder;

	// 1. Update environment & gravity
	if (chBelow == Board::EMPTY || chBelow == Hammer::HAMMER_ICON || chBelow == Pauline::PAULINE_ICON ||
		chBelow == SmallGhost::SMALL_GHOST_ICON || chBelow == BigGhost::BIG_GHOST_ICON ||
		chBelow == Barrel::BARREL_ICON || chBelow == DonkeyKong::DONKEY_KONG_ICON) { // Case: Mario is falling or jumping
		isOnGround = false;
		onLadder = false;
		if (!jumping) 
			falling = true;
	}
	else if (Tiles::isTile(chBelow) && prevCh != Board::LADDER) { // Case: Mario is on the ground
		isOnGround = true;
		onLadder = false;
		falling = false;
		jumpCounter = 0;
	}
	else if (chBelow == Board::LADDER) { // Case: Mario is on the ladder
		isOnGround = false;
		onLadder = true;
		falling = false;
	}
	else if (chAbove == Board::LADDER) 
		falling = false;

	// 2. Ladder dismount intercept
	if (wasOnLadder && isOnGround && !onLadder) {
		if (pressedKey == Key::Up) 
			pressedKey = Key::Stay;

		canJump = false;
	}

	// 3. Update ongoing jump
	if (jumping && (jumpCounter >= MAX_JUMP_HEIGHT || Tiles::isTile(chAbove) || chAbove == Board::WALL)) {
		jumping = false;
		falling = true;
		jumpCounter = 0;
	}

	// 4. Handle new inputs
	if (isOnGround && pressedKey == Key::Up && !jumping && canJump && prevCh != Board::LADDER) { // Case: start jumping
		onLadder = false;
		falling = false;
		jumping = true;

		if (currDirX == -1)
			pressedKey = Key::Left;
		else if (currDirX == 1)
			pressedKey = Key::Right;
		else
			pressedKey = Key::Stay;

		canJump = false; // Consumes the jump without erasing Mario X-axis movement
	}
	else if (prevCh == Board::LADDER && isOnGround && pressedKey == Key::Up) { // Case: climbing a ladder from a floor
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
}

bool Mario::isValidToMove() {
	int nextPosX = x + currDirX; // Direction in x axis
	int nextPosY = y + currDirY; // Direction in y axis

	if (!board.isWithinBounds(nextPosX, nextPosY)) 
		return false;

	char nextCh = board.getBoardChar(nextPosX, nextPosY); // Update next char

	// Handle movement based on the next char

	if (Tiles::isTile(nextCh)) { // Tiles
		if (onLadder && pressedKey == Key::Up)
			return true;
		if (isOnGround && pressedKey == Key::Down && board.isWithinBounds(x, y + 2) && board.getBoardChar(x, y + 2) == Board::LADDER) // Case: go down the ladder through a wall
			return true;
		
		return false; // Case: Mario cannot walk into tiles
	}

	switch (nextCh) {
	case Board::EMPTY: // Empty space
		if (jumping || falling) // Mario is in an uncontrollable y axis state
			return true;
		if (isOnGround && currDirY == 0) // Case: Mario can move to empty space if on the ground
			return true;
		if (onLadder) // Case: Mario can move to empty space if on ladder
			return true;
		return false;
	case Board::LADDER:
		return true;
	case Board::WALL:
		return false; // Mario can't move through walls
	case Barrel::BARREL_ICON:
		return true;
	case Pauline::PAULINE_ICON:
		return true;
	case Hammer::HAMMER_ICON:
		return true;
	case ExtraLife::EXTRA_LIFE_ICON:
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
	if (hasHammer() && pressedKey == Key::PlayHammer)
		useHammer();

	calculatePrevPos();
	eraseFromBoard();
	eraseFromConsole();

	updateState();
	tryMove();

	if (isOnGround && fallCounter >= MAX_FALL_COUNTER) {
		isDead = true;
		resetDir();
	}
	else if (isOnGround || onLadder) // Case: Survived the landing, clear the damage
		fallCounter = 0;

	if (!isDead) {
		bool canMove = isValidToMove();

		if (!canMove && falling && currDirX != 0) { // Diagonal Wall Collision Fallback
			currDirX = 0;
			canMove = isValidToMove();
		}

		if (canMove) {
			int nextPosX = x + currDirX; // Direction in x axis
			int nextPosY = y + currDirY; // Direction in y axis

			if (jumping)
				jump();
			else if (falling)
				fall();
			else { // Case: Mario can walk normally or climb
				x = nextPosX;
				y = nextPosY;
			}
		}
		else // Mario cannot move
			resetDir();
	}

	drawToBoard();
	drawToConsole();

	if (hasHammer())
		hammer->updateState();
}

void Mario::fall() {
	// Update direction to fall
	currDirY = 1;

	int nextPosX = x + currDirX; // Direction in x axis
	int nextPosY = y + currDirY; // Direction in y axis

	if (board.isWithinBounds(nextPosX, nextPosY)) {
		char chBelow = board.getBoardChar(nextPosX, nextPosY);

		if (chBelow == Board::EMPTY || chBelow == Hammer::HAMMER_ICON ||chBelow == ExtraLife::EXTRA_LIFE_ICON ||
			chBelow == Pauline::PAULINE_ICON || chBelow == SmallGhost::SMALL_GHOST_ICON || chBelow == BigGhost::BIG_GHOST_ICON ||
			chBelow == Barrel::BARREL_ICON || chBelow == DonkeyKong::DONKEY_KONG_ICON) { // On air
			y += currDirY;
			x += currDirX;
			fallCounter++;
		}
		else if (Tiles::isTile(chBelow) || chBelow == Board::WALL) { // Going to hit ground after falling
			isOnGround = true;
			currDirY = 0;
			fallCounter = 0;
			jumpCounter = 0;
		}
		else {
			resetDir();
			fallCounter = 0;
		}
	}
	else { // Case: hitting the bottom of the board
		isOnGround = true;
		currDirY = 0;
	}
}

void Mario::jump() {
	currDirY = -1;

	if (jumpCounter < MAX_JUMP_HEIGHT) {
		int nextPosX = x + currDirX;
		int nextPosY = y + currDirY;

		if (board.isWithinBounds(nextPosX, nextPosY)) {
			char nextCh = board.getBoardChar(nextPosX, nextPosY);

			if (nextCh == Board::EMPTY || nextCh == Board::LADDER ||
				nextCh == Hammer::HAMMER_ICON || nextCh == ExtraLife::EXTRA_LIFE_ICON || nextCh == Pauline::PAULINE_ICON) {
				x = nextPosX;
				y = nextPosY;
				++jumpCounter;
			}

			else if (Tiles::isTile(nextCh) || nextCh == Board::WALL || nextCh == Barrel::BARREL_ICON ||
				nextCh == SmallGhost::SMALL_GHOST_ICON || nextCh == BigGhost::BIG_GHOST_ICON || nextCh == DonkeyKong::DONKEY_KONG_ICON) {
				jumping = false;
				falling = true;
				jumpCounter = 0;
			}
		}
		else {
			jumping = false;
			jumpCounter = MAX_JUMP_HEIGHT;
		}
	}
}

void Mario::useHammer() {
	if (hammer == nullptr)
		return;

	bool swingStarted = false; // Local variable instead of a class member
	hammer->use(x, y, currDirX, &swingStarted);
	if (swingStarted)
		pressedKey = Key::Init; // Reset pressed key after using the hammer
}

void Mario::reset() {
	// Reset position
	resetDir();
	x = startPosX;
	y = startPosY;
	prevCh = Board::EMPTY;

	// Reset input
	pressedKey = Key::Init;

	// Hammer state
	delete hammer;
	hammer = nullptr;
	icon = MARIO_ICON;

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

void Mario::pickUpLife() {
	++life;
	prevCh = Board::EMPTY;
}

void Mario::tryPickUpHammer(Hammer*& uncollectedHammer) {
	if (uncollectedHammer != nullptr) {
		if (uncollectedHammer->getX() == x && uncollectedHammer->getY() == y) {
			if (!hasHammer()) 
				pickUpHammer(uncollectedHammer);
			else {
				delete uncollectedHammer;
				prevCh = Board::EMPTY; // Clear Mario's footprint so he doesn't leave a ghost hammer behind
			}
			uncollectedHammer = nullptr; // Remove the hammer from the board
		}
	}
}

void Mario::pickUpHammer(Hammer* h) {
	hammer = h;
	hammer->setCollected();
	icon = MARIO_HAMMER_ICON;
	prevCh = Board::EMPTY;
}