#pragma once
#include <vector>
#include "GameManager.h"
#include "Colors.h"
#include "Level.h"

class Board {

	const Level* currentOriginalLevel;
	std::vector<std::string> gameBoard;

public:
	static constexpr char LADDER = 'H';
	static constexpr const char* LADDER_COLOR = CYAN;
	static constexpr char EMPTY = ' ';
	static constexpr char WALL = 'Q';
	static constexpr const char* WALL_COLOR = BRIGHT_RED;
	static constexpr const char* TILES_COLOR = BRIGHT_RED;

	// Board state
	void reset(unsigned int marioLife);
	void print() const;

	// Edge case handling
	bool isWithinBounds(int x, int y) const { return (GameManager::MIN_X <= x && x < GameManager::MAX_X && GameManager::MIN_Y <= y && y < GameManager::MAX_Y); }

	// Getters & Setters
	char getBoardChar(int x, int y) const;
	void setBoardChar(int x, int y, char ch);
	Level getLevel() { return *currentOriginalLevel; }
	void setLevel(const Level* newLevel);
};
