#pragma once
#include <cstring>
#include "Item.h"
#include "Colors.h"

// Forward declaration
class Board;

class Hammer : public Item {
	// Swing position tracking
	int hammerPosX[2];
	int hammerPosY;
	char prevHammerChars[2];

	// Hammer state machine management
	enum class HammerState { Uncollected, Idle, HalfDeployed, FullyDeployed, RestoreAndIdle };
	HammerState state;

	// Drawing
	void draw(int radius) const;

	// Erasing
	void grab() const;
	void eraseHammerCharsFromBoard(int radius) const;
	void eraseHammerCharsFromConsole(int radius) const;

public:
	Hammer(int x, int y, Board& b, bool& isColor) : Item(x, y, HAMMER_ICON, HAMMER_COLOR, b, isColor) { // Constructor
		std::memset(hammerPosX, 0, sizeof(hammerPosX));
		std::memset(prevHammerChars, '\0', sizeof(prevHammerChars));
		hammerPosY = -1;
		state = HammerState::Uncollected;
	}

	// Constants
	static constexpr char HAMMER_ICON = 'p';
	static constexpr const char* HAMMER_COLOR = BROWN;

	// Swing actions
	void use(int marioX, int marioY, int xDirection, bool* hammerUsed);
	void updateState();

	// Getters
	int getHitX(int index) const { return hammerPosX[index]; }
	int getHitY() const { return hammerPosY; }
	bool isSwinging() const { return state == HammerState::HalfDeployed || state == HammerState::FullyDeployed
			|| state == HammerState::RestoreAndIdle;
	}

	// Setter
	void marioPickedUp() { state = HammerState::Idle; }
};