#pragma once
#include <algorithm> // for std::find
#include <array>

class Tiles {
	// Constant
	static constexpr std::array<char, 3> tiles = {'<', '=', '>'};

public:
	Tiles() = delete; // Constructor

	// Constant
	static constexpr char OUT_OF_BOUNDS_FALLBACK_FLOOR = tiles[1];

	// Tile verifier
	static bool isTile(char ch) { return std::ranges::find(tiles, ch) != tiles.end(); }
};