#pragma once
#include "Character.h"
#include "Hammer.h"

// Forward declaration
class Board;

class Mario : public Character {

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

	// Constants
	static constexpr const char* MARIO_COLOR = RED;
	static constexpr const char* HAMMER_COLOR = BLUE;
	static constexpr int MAX_JUMP_HEIGHT = 2;
	static constexpr int MAX_FALL_COUNTER = 5;

	// Mario's position and movement
	int startPosX;
	int startPosY;
	Key pressedkey;

	// Life
	unsigned int life;

	// Hammer handling
	bool withHammer;
	bool hammerUsed;

	// Jumping & Falling
	unsigned int jumpCounter;
	unsigned int fallCounter;
	bool isOnGround;
	bool jumping;
	bool onLadder;
	bool falling;

	// Hammer
	Hammer* hammer;

public:
	Mario(int x, int y, Board& b, bool& isColor) : Character(x, y, MARIO_ICON, isColor, b),
		prevPosX(0), prevPosY(0), startPosX(x), startPosY(y), pressedkey(Key::Stay),
		life(3),
		withHammer(false), hammerUsed(false),
		jumpCounter(0), fallCounter(0), isOnGround(true), jumping(false), onLadder(false), falling(false),
		hammer(nullptr)	{}

	static constexpr char MARIO_ICON = '@';
	static constexpr char MARIO_HAMMER_ICON = 'M';
	static constexpr char HAMMER_ICON = 'P';

	int getMarioX() const { return x; }
	int getMarioY() const { return y; }

	char getMarioIcon() const { return icon; }

	void setPressedKey(char ch);
	void updateState();
	bool isValidToMove() override;
	void resetDir() { currDirX = 0;	currDirY = 0; }
	void move();
	void reset();

	// Jumping & Falling
	void jump();
	void fall();

	// Life management
	unsigned int marioLifePoints() const { return life; }
	void decreaseLife() { --life; }

	// Hammer management
	void pickUpHammer(Hammer* h) { hammer = h; withHammer = true; hammer->setCollected(); icon = MARIO_HAMMER_ICON; }
	void useHammer();
	void resetHammer();
};