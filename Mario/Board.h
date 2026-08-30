#pragma once
#include <vector>
#include "GameManager.h"
#include "Colors.h"
#include "Level.h"

class Board {
	// Constants
	static constexpr const char* LADDER_COLOR = CYAN;
	static constexpr const char* WALL_COLOR = PINK;
	static constexpr const char* TILES_COLOR = PINK;

	// Level data
	const Level* currentOriginalLevel = nullptr;
	std::vector<std::string> gameBoard;

	// Silent mode
	bool silentMode = false;

public:
	// Constants
	static constexpr int MIN_X = 0;
	static constexpr int MIN_Y = 0;
	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;
	static constexpr char LADDER = 'H';
	static constexpr char EMPTY = ' ';
	static constexpr char WALL = 'Q';

	// Board state
	void reset();
	void print(bool isColor, const Legend& legend) const;
	static void printCharWithColor(char ch);

	// Edge case handling
	bool isWithinBounds(int x, int y) const { return (MIN_X <= x && x < MAX_X && MIN_Y <= y && y < MAX_Y); }

	// Silent handling
	void setSilent(bool silent) { silentMode = silent; }
	bool isSilent() const { return silentMode; }

	// Getters & Setters
	char getBoardChar(int x, int y) const;
	const Level& getLevel() const { return *currentOriginalLevel; }

	// Setters
	void setBoardChar(int x, int y, char ch);
	void setLevel(const Level* newLevel);
};