#pragma once
#include <cstring>
#include "Colors.h"
#include "MovingEnemy.h"

class Barrel : public MovingEnemy {
	// Constant
	static constexpr unsigned int EXPLOSION_DELAY_MS = 50;

	// Explosion state
	enum class ExplosionState { NotExploding, Radius1, Radius2, RestoreAndDie };
	ExplosionState explosionState;

	// State variables
	unsigned int fallCounter;
	bool isOnAir;
	char prevChars[3][5];
	int lastDir_x;

	Barrel(int xPos, int yPos, Board& b, bool& isColorOfBarrel) : MovingEnemy(xPos, yPos, BARREL_ICON, BARREL_COLOR, b, isColorOfBarrel),
		fallCounter(0), isOnAir(false), lastDir_x(0) { // Constructor
		std::memset(prevChars, '\0', sizeof(prevChars));
		explosionState = ExplosionState::NotExploding;
	}

	// Barrel movement
	void setDirection();
	void fall();

	// Explosion FX
	void startExplode() { explosionState = ExplosionState::Radius1; }
	void processExplosion();
	void savePreviousCharsExplosion();
	void create_first_radius_exp() const;
	void delete_first_radius_exp() const;
	void create_second_radius_exp() const;
	void restorePrevChars() const;

	friend class BarrelFactory;

public:
	// Constants
	static constexpr char BARREL_ICON = 'O';
	static constexpr const char* BARREL_COLOR = BROWN;

	// Barrel movement
	void move() override;

	// Getter
	bool isExploded() const { return isDead; }
};