#pragma once
#include <vector>
#include "Game.h"

class Board {

	std::vector<std::string> level;

public:
	static constexpr char LADDER = 'H';
	inline static constexpr const char* LADDER_COLOR = CYAN;
	static constexpr char EMPTY = ' ';
	static constexpr char WALL = 'Q';
	inline static constexpr const char* WALL_COLOR = BRIGHT_RED;
	inline static constexpr const char* TILES_COLOR = BRIGHT_RED;
	
	bool isWithinBounds(int x, int y) const { return (Game::MIN_X <= x && x < Game::MAX_X && Game::MIN_Y <= y && y < Game::MAX_Y); }
	char getBoardChar(int x, int y) const { return level[y][x]; }
	void setBoardChar(int x, int y, char ch) { level[y][x] = ch; }
};

