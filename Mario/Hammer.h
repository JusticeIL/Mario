#pragma once
#include <cstring>
#include "Item.h"
#include "Colors.h"

// Forward declaration
class Board;

class Hammer : public Item {

	static constexpr const char* HAMMER_COLOR = BROWN;
	int hammerPosX[2];
	int hammerPosY;
	char prevHammerChars[2];

	// Hammer state machine management
	enum class HammerState { Uncollected, Idle, HalfDeployed, FullyDeployed, RestoreAndIdle };
	HammerState state;

public:
	Hammer(int x, int y, Board& b, bool& isColor) : Item(x, y, HAMMER_ICON, b, isColor) {
		std::memset(hammerPosX, 0, sizeof(hammerPosX));
		std::memset(prevHammerChars, '\0', sizeof(prevHammerChars));
		hammerPosY = -1;
		state = HammerState::Uncollected;
	}

	static constexpr char HAMMER_ICON = 'p';

	void use(int marioX, int marioY, int xDirection, bool* hammerUsed);
	void updateState();

	void grab() const ;
	void draw(int radius) const;
	void eraseHammerCharsFromBoard(int radius) const;
	void eraseHammerCharsFromConsole(int radius) const;
	void reset();

	void setCollected() override { Item::setCollected(); state = HammerState::Idle; }

	// Getters
	int getHitX(int index) const { return hammerPosX[index]; }
	int getHitY() const { return hammerPosY; }
	bool isSwinging() const { return state == HammerState::HalfDeployed || state == HammerState::FullyDeployed
			|| state == HammerState::RestoreAndIdle;
	}
};