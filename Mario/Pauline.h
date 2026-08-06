#pragma once
#include "Character.h"
#include "Colors.h"

// Forward declaration
class Board;

class Pauline : public Character {

	static constexpr const char* PAULINE_COLOR = PINK;
	bool hasMarioArrived;

public:
	Pauline(int x, int y, Board& b, bool& isColor) : Character(x, y, PAULINE_ICON, b, isColor), hasMarioArrived(false) {} // Constructor

	static constexpr char PAULINE_ICON = '$';
	bool checkWinCondition() const { return hasMarioArrived; }
	void updateWinCondition(int marioX, int marioY);
};