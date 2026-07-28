#pragma once
#include "Character.h"

class Pauline : public Character {

	static constexpr const char* PAULINE_COLOR = PINK;
	bool hasMarioArrived;

public:
	Pauline(int x, int y, bool& isColor, Board& b) : Character(x, y, PAULINE_ICON, isColor, b), hasMarioArrived(false) {} // Constructor

	static constexpr char PAULINE_ICON = '$';
	bool checkWinCondition() { return hasMarioArrived; }
	void updateWinCondition(int marioX, int marioY);
	void reset() { hasMarioArrived = false; }
};