#pragma once
#include "Ghost.h"
#include <random>
#include "Colors.h"


class SmallGhost : public Ghost {

	// Constants
	static constexpr const char* SMALL_GHOST_COLOR = RED;

	// Seed variables
	long int savedSeed; // Stores the seed; 0 means uninitialized
	std::mt19937 gen; // Random number generator
	std::uniform_int_distribution<int> dist; // Uniform distribution

	void changeGhostDirection() { currDirX = currDirX * (-1); }

public:
	SmallGhost(int x, int y, bool& isColor, Board& b, unsigned int seed) : Ghost(x, y, SMALL_GHOST_ICON, isColor, b),
		savedSeed(0), gen(seed), dist(1, 100) {
		currDirX = 1; // Initial direction to the right after parent initialization
	}

	static constexpr char SMALL_GHOST_ICON = 'x';

	void move() override;
	unsigned int getRandomNumber() { return dist(gen); }
	void setDirection();
};