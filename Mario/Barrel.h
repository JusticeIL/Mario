#pragma once
#include "MovingEnemy.h"

class Barrel : public MovingEnemy {

	static constexpr unsigned int EXPLOSION_DELAY_MS = 50;

	// State variables
	unsigned int fallCounter;
	bool isOnAir;
	bool isExploded;
	int lastDir_x;
	char prevChars[3][5];

	Barrel(int x, int y, bool& isColor, Board& b) : MovingEnemy(x, y, BARREL_ICON, isColor, b),
		fallCounter(0), isOnAir(false), isExploded(false), lastDir_x(0)	{
		std::memset(prevChars, '\0', sizeof(prevChars));
	}

	// Explosion FX
	void savePreviousCharsExplosion();
	void create_first_radius_exp() const;
	void delete_first_radius_exp() const;
	void create_second_radius_exp() const;
	void restorePrevChars() const;

	friend class BarrelFactory;

public:
	static constexpr char BARREL_ICON = 'O';

	// Barrel movement
	void move() override;
	void setDirection();
	void fall();

	void explode();
};