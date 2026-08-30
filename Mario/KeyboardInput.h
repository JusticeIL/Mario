#pragma once
#include <random>
#include "InputProvider.h"

class KeyboardInput : public InputProvider {
public:
	// Input playback
	void loadLevelInput(const std::string& levelName) override { return; }
	char getInput(int currentTick) override;

	// Getters
	bool allowEsc() const override { return true; }
	unsigned int getSeed() override { std::random_device rd; return rd(); }
	unsigned int getRefreshRate() const override { return 0; }
};