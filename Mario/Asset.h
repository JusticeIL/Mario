#pragma once

// Forward declaration
class Board;

class Asset {

protected:
	int prevPosX;
	int x;
	int currDirX;
	int prevPosY;
	int y;
	int currDirY;
	char prevCh;
	char icon;
	const bool& isColor;
	const char* color;

	// Board
	Board& board;

	Asset(int x, int y, char symbol, const char* color, Board& b, bool& isColor);

public:
	virtual ~Asset() {
		eraseFromBoard();
		eraseFromConsole();
	}

	void calculatePrevPos();
	virtual void drawToConsole() const = 0;
	virtual void drawToBoard() = 0;
	void eraseFromConsole() const;
	void eraseFromBoard() const;

	// Getters
	int getX() const { return x; }
	int getY() const { return y; }
	char getIcon() const { return icon; }
};