#pragma once
#include <vector>
#include "Game.h"

class Board {

	std::vector<std::string> level;

public:
	static constexpr char ladder = 'H';
	static constexpr char empty = ' ';
	
	bool isWithinBounds(int x, int y) const { return (Game::MIN_X <= x && x < Game::MAX_X && Game::MIN_Y <= y && y < Game::MAX_Y); }
	char getBoardChar(int x, int y) const { return level[y][x]; }
};

