#pragma once
class Asset {

protected:
	int x;
	int y;
	const char icon;
	const bool& isColor;

public:
	 Asset(int x, int y, char symbol, bool& isColor) : x(x), y(y), icon(symbol), isColor(isColor) {}
	 void draw() const;
};