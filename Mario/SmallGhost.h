#pragma once
#include "Ghost.h"
#include <random>
#include "Colors.h"

class SmallGhost : public Ghost {

	// Constants
	static constexpr const char* SMALL_GHOST_COLOR = RED;

	// Seed variables
	unsigned int savedSeed;
	std::mt19937 gen; // Random number generator
	std::uniform_int_distribution<int> dist; // Uniform distribution

	void changeGhostDirection() { currDirX = currDirX * (-1); }

public:
	SmallGhost(int x, int y, Board& b, bool& isColor, unsigned int seed) : Ghost(x, y, SMALL_GHOST_ICON, b, isColor),
		savedSeed(seed), gen(seed), dist(1, 100) {
		currDirX = 1; // Initial direction to the right after parent initialization
	}

	static constexpr char SMALL_GHOST_ICON = 'x';

	bool isValidToMove() override;
	void move() override;
	unsigned int getRandomNumber() { return dist(gen); }
	void setDirection();
};