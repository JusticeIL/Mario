#pragma once
#include <cstring>
#include "MovingEnemy.h"

class Barrel : public MovingEnemy {

	static constexpr unsigned int EXPLOSION_DELAY_MS = 50;

	// State variables
	unsigned int fallCounter;
	bool isOnAir;
	int lastDir_x;
	char prevChars[3][5];

	// Explosion state
	enum class ExplosionState {	NotExploding, Radius1, Radius2, RestoreAndDie };
	ExplosionState explosionState;

	Barrel(int x, int y, Board& b, bool& isColor) : MovingEnemy(x, y, BARREL_ICON, b, isColor),
		fallCounter(0), isOnAir(false), lastDir_x(0) { // Constructor
		std::memset(prevChars, '\0', sizeof(prevChars));
		explosionState = ExplosionState::NotExploding;
	}

	// Barrel movement
	void setDirection();
	void fall();

	// Explosion FX
	void processExplosion();
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

	// Barrel explosion handling
	void startExplode() { explosionState = ExplosionState::Radius1; }
	bool isExploded() const { return isDead; }
};