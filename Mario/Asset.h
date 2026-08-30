#pragma once

// Forward declaration
class Board;

class Asset {

protected:
	// Position
	int x;
	int y;

	// Direction and movement control
	int currDirX;
	int currDirY;

	// Previous position tracking
	int prevPosX;
	int prevPosY;
	char prevCh;

	// Appearance
	const char* color;
	const bool& isColor;
	char icon;

	// Game board reference
	Board& board;

	Asset(int x, int y, char symbol, const char* color, Board& b, bool& isColor);

	// Saves the current position before the asset moves
	void calculatePrevPos();

	// Erasing
	void eraseFromConsole() const;
	void eraseFromBoard() const;

public:
	virtual ~Asset() {
		eraseFromBoard();
		eraseFromConsole();
	}

	// Drawing
	virtual void drawToConsole() const = 0;
	virtual void drawToBoard() = 0;

	// Getters
	int getX() const { return x; }
	int getY() const { return y; }
	char getIcon() const { return icon; }
};