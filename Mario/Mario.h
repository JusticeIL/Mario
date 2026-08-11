#pragma once
#include "Character.h"
#include "Hammer.h"

// Forward declaration
class Board;

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

	// Mario's position and movement
	int startPosX;
	int startPosY;
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

public:
	Mario(int x, int y, Board& b, bool& isColor) : Character(x, y, MARIO_ICON, MARIO_COLOR, b, isColor),
		startPosX(x), startPosY(y), pressedKey(Key::Stay),
		life(INIT_LIFE),
		jumpCounter(0), fallCounter(0), isOnGround(true), canJump(true), jumping(false), onLadder(false), falling(false),
		hammer(nullptr)	{
		prevPosX = x;
		prevPosY = y;
		hammer = nullptr;
	}

	static constexpr char MARIO_ICON = '@';
	static constexpr char MARIO_HAMMER_ICON = 'M';
	static constexpr char HAMMER_ICON = 'P';

	// Getters
	int& getMarioXRef() { return x; }
	int& getMarioYRef() { return y; }
	int getMarioX() const { return x; }
	int getMarioY() const { return y; }
	char getMarioIcon() const { return icon; }

	// Setters
	void setSpawnPoint(int newX, int newY) { startPosX = newX; startPosY = newY; }
	void setPressedKey(char ch);

	void updateState();
	bool isValidToMove();
	void resetDir() { currDirX = 0;	currDirY = 0; }
	void tryMove();
	void move();
	void reset();

	// Jumping & Falling
	void jump();
	void fall();

	// Life management
	unsigned int marioLifePoints() const { return life; }
	const unsigned int& getMarioLifeRef() const { return life; }
	void decreaseLife() { --life; }
	void restoreLives() { life = INIT_LIFE; }

	// Hammer management
	void tryPickUpHammer(Hammer*& uncollectedHammer);
	void pickUpHammer(Hammer* h);
	void useHammer();
	void resetHammer() const { hammer->reset(); }
	bool hasHammer() const { return hammer != nullptr; }
	Hammer* getHammer() const { return hammer; }
};