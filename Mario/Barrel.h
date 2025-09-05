#pragma once
#include "Enemy.h"
#include "Colors.h"
#include <list>

class Barrel : public Enemy {

	static constexpr unsigned int EXPLOSION_DELAY_MS = 50;

	// State variables
	unsigned int fallCounter;
	bool isOnAir;
	bool isExploded;
	int lastDir_x;
	char prevChars[3][5];

	Barrel(int x, int y, bool& isColor, Board& b) : Enemy(x, y, BARREL_ICON, isColor, b),
		fallCounter(0), isOnAir(false), isExploded(false), lastDir_x(0)	{
		std::memset(prevChars, '\0', sizeof(prevChars));
	}

	// Explosion FX
	void savePreviousCharsExplosion();
	void create_first_radius_exp();
	void delete_first_radius_exp();
	void create_second_radius_exp();
	void restorePrevChars();

	friend class BarrelFactory;

public:
	static constexpr char BARREL_ICON = 'O';

	// Barrel movement
	void move() override;
	bool isValid() override;
	void setDirection();
	void fall(bool canBarrelMove);

	void explode();
};