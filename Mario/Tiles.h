#pragma once
#include <array>
#include <algorithm> // for std::find

class Tiles {

	static constexpr std::array<char, 3> tiles = {'<', '=', '>'};
	const bool& isColor;

public:
	Tiles(bool& isColor) : isColor(isColor) {}
	static bool isTile(char ch) { return std::find(tiles.begin(), tiles.end(), ch) != tiles.end(); }
	static constexpr char OUT_OF_BOUNDS_FALLBACK_FLOOR = tiles[1];
};