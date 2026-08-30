#pragma once
#include "Character.h"
#include "Hammer.h"

// Forward declarations
class Board;
class Legend;

// Define available keys here:
enum class Key : char {
	Up = 'w',
	Left = 'a',
	Down = 'x',
	Right = 'd',
	Stay = 's',
	PlayHammer = 'p',
	Init = '\0'
};

class Mario : public Character {
	// Constants
	static constexpr const char* MARIO_COLOR = MARIO_PEACH;
	static constexpr int MAX_JUMP_HEIGHT = 2;
	static constexpr int MAX_FALL_COUNTER = 5;
	static constexpr unsigned int INIT_LIFE = 3;

	// Spawn point
	int startPosX;
	int startPosY;

	// Mario movement
	Key pressedKey;

	// Life
	unsigned int life;

	// Jumping & Falling
	unsigned int jumpCounter;
	unsigned int fallCounter;
	bool isOnGround;
	bool canJump;
	bool jumping;
	bool onLadder;
	bool falling;

	// Hammer
	Hammer* hammer;

	// Movement
	void tryMove();
	void updateState();
	bool isValidToMove() const;
	void resetDir() { currDirX = 0;	currDirY = 0; }

	// Jumping & Falling
	void jump();
	void fall();

	// Hammer management
	void pickUpHammer(Hammer* h);
	void useHammer();

public:
	Mario(int x, int y, Board& b, bool& isColor) : Character(x, y, MARIO_ICON, MARIO_COLOR, b, isColor),
		startPosX(x), startPosY(y), pressedKey(Key::Stay),
		life(INIT_LIFE),
		jumpCounter(0), fallCounter(0), isOnGround(true), canJump(true), jumping(false), onLadder(false), falling(false),
		hammer(nullptr)	{ // Constructor
		prevPosX = x;
		prevPosY = y;
		hammer = nullptr;
	}
	~Mario() override { delete hammer; } // Destructor

	// Constants
	static constexpr char MARIO_ICON = '@';
	static constexpr char MARIO_HAMMER_ICON = 'M';
	static constexpr char HAMMER_ICON = 'P';

	// Mario actions
	void move();
	void reset();

	// Jumping & Falling
	bool isJumping() const { return jumping; }
	void bumpHead() { jumping = false; falling = true; jumpCounter = 0; }

	// Life management
	unsigned int marioLifePoints() const { return life; }
	const unsigned int& getMarioLifeRef() const { return life; }
	void decreaseLife() { --life; }
	void pickUpLife();
	void restoreLives() { life = INIT_LIFE; }

	// Hammer management
	void tryPickUpHammer(Hammer*& uncollectedHammer);
	bool hasHammer() const { return hammer != nullptr; }
	Hammer* getHammer() const { return hammer; }

	// Getters
	int& getMarioXRef() { return x; }
	int& getMarioYRef() { return y; }
	int getMarioX() const { return x; }
	int getMarioY() const { return y; }

	// Setters
	void setSpawnPoint(int newX, int newY) { startPosX = newX; startPosY = newY; }
	void setPressedKey(char key);
};