#pragma once
#include <string>
#include "Barrel.h"
#include "Board.h"

class BarrelFactory {

	bool isColor;
	const int creationPosX;
	const int creationPosY;

public:
	BarrelFactory(int dkx, int dky, bool& isColor, std::string level) : isColor(isColor) {} // TODO: implement checking dk pos and decide x,y factory coordinates accordingly
	Barrel* createBarrel(int x, int y, bool& isColor) {	return new Barrel(x, y, isColor); }
};