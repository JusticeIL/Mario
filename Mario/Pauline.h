#pragma once
#include "Character.h"

class Pauline : public Character {

	

public:
	Pauline(int x, int y, bool& isColor) : Character(x, y, PAULINE_ICON, isColor) {}

	static constexpr char PAULINE_ICON = '$';
};