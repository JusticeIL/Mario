#pragma once
#include "Character.h"
#include <set>

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
	static constexpr char MARIO_ICON = '@';
	static constexpr char MARIO_HAMMER_ICON = 'M';
	inline static constexpr const char* MARIO_COLOR = RED;
	static constexpr char HAMMER_ICON = 'P';
	inline static constexpr const char* HAMMER_COLOR = BLUE;
	static constexpr int MAX_JUMP_HEIGHT = 2;
	static constexpr int MAX_FALL_COUNTER = 5;

	// Mario's position and movement
	int currDirX;
	int currDirY;
	int prevPosX;
	int prevPosY;
	int startPosX;
	int startPosY;
	char prevCh;
	Key pressedkey;

	// Hammer handling
	bool withHammer;
	bool hammerUsed;

	// Jumping & Falling
	int jumpCounter;
	int fallCounter;
	bool isOnGround;
	bool jumping;
	bool onLadder;
	bool falling;

	// Game state
	bool dead;
	bool winCon;

	// Board
	const Board& board;

	// Hammer
	Hammer* hammer;

public:
	Mario(int x, int y, Board& b, bool& isColor) : Character(x, y, MARIO_ICON, isColor), board(b),
		currDirX(0), currDirY(0), prevPosX(0), prevPosY(0), startPosX(x), startPosY(y), prevCh(Board::EMPTY), pressedkey(Key::Stay),
		withHammer(false), hammerUsed(false),
		jumpCounter(0), fallCounter(0), isOnGround(true), jumping(false), onLadder(false), falling(false),
		dead(false), winCon(false),
		hammer(nullptr)	{}

	int getMarioX() const { return x; }
	int getMarioY() const { return y; }

	void setPressedKey(char ch);
	void updateState();
	bool isValid();
	void resetDir() { currDirX = 0;	currDirY = 0; }
	void move();
	void reset();

	// Jumping & Falling
	void jump();
	void fall();

	// Hammer management
	void useHammer();
	void resetHammer();
};