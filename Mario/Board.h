#pragma once
#include <vector>
#include "GameManager.h"
#include "Colors.h"

class Board {

	Level currentOriginalLevel;
	std::vector<std::string> gameBoard;

public:
	static constexpr char LADDER = 'H';
	static constexpr const char* LADDER_COLOR = CYAN;
	static constexpr char EMPTY = ' ';
	static constexpr char WALL = 'Q';
	static constexpr const char* WALL_COLOR = BRIGHT_RED;
	static constexpr const char* TILES_COLOR = BRIGHT_RED;

	// Board state
	void reset() { gameBoard = currentOriginalLevel.getOriginalLevel(); }
	void print() const;

	bool isWithinBounds(int x, int y) { return (GameManager::MIN_X <= x && x < GameManager::MAX_X && GameManager::MIN_Y <= y && y < GameManager::MAX_Y); }
	char getBoardChar(int x, int y) const { return gameBoard[y][x]; }
	void setBoardChar(int x, int y, char ch) { gameBoard[y][x] = ch; }
	Level getLevel() { return currentOriginalLevel; }
};