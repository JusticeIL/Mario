#pragma once
#include "Ghost.h"
#include <random>

class SmallGhost : public Ghost {

	// Seed variables
	std::mt19937 gen; // Random number generator
	std::uniform_int_distribution<int> dist; // Uniform distribution

	// Direction handling
	void setDirection();
	void changeGhostDirection() { currDirX = currDirX * (-1); }

	// Random number generation
	unsigned int getRandomNumber() { return dist(gen); }

protected:
	// Validation
	bool isValidToMove() override;

public:
	SmallGhost(int x, int y, Board& b, bool& isColor, unsigned int seed) : Ghost(x, y, SMALL_GHOST_ICON, b, isColor),
		gen(seed), dist(1, 100) { // Constructor
		currDirX = 1; // Initial direction to the right after parent initialization
	}

	// Constant
	static constexpr char SMALL_GHOST_ICON = 'x';

	// Ghost Action
	void move() override;
};