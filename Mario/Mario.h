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
		Stay = 's'
	};

	// Constants
	static constexpr char MARIO_ICON = '@';
	static constexpr char HAMMER_ICON = 'P';
	static constexpr int MAX_JUMP_HEIGHT = 2;
	static constexpr int MAX_FALL_COUNTER = 5;

	// Mario's position and movement
	int currDirX;
	int currDirY;
	int prevPosX;
	int prevPosY;
	char prevCh;
	char chBelow;
	Key pressedkey;

	// Hammer handling
	int hammerPos_x[2];
	int hammerPos_y[2];
	char prevHammerChars[3];
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

public:
	Mario(int x, int y, Board& b, bool& isColor) : Character(x, y, MARIO_ICON, isColor), board(b),
		currDirX(0), currDirY(0), prevPosX(0), prevPosY(0),prevCh(Board::EMPTY), chBelow('\0'), pressedkey(Key::Stay),
		withHammer(false), hammerUsed(false),
		jumpCounter(0), fallCounter(0), isOnGround(true), jumping(false), onLadder(false), falling(false),
		dead(false), winCon(false)	{

		std::memset(hammerPos_x, 0, sizeof(hammerPos_x));
		std::memset(hammerPos_y, 0, sizeof(hammerPos_y));
		std::memset(prevHammerChars, '\0', sizeof(prevHammerChars));
	}

	int getMarioX() const { return x; }
	int getMarioY() const { return y; }

	void updateState();
	bool isValid();
	void resetDir() { currDirX = 0;	currDirY = 0; }
	void move();
	
};