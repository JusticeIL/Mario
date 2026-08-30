#pragma once
#include "Character.h"
#include "Colors.h"

// Forward declaration
class Board;

class Pauline : public Character {
	// Data member
	bool hasMarioArrived;

public:
	Pauline(int x, int y, Board& b, bool& isColor) : Character(x, y, PAULINE_ICON, PAULINE_COLOR, b, isColor), hasMarioArrived(false) {} // Constructor

	// Constants
	static constexpr char PAULINE_ICON = '$';
	static constexpr const char* PAULINE_COLOR = PINK;

	// Win condition
	bool checkWinCondition() const { return hasMarioArrived; }
	void updateWinCondition(int marioX, int marioY);
};