#pragma once
#include <utility>
#include <vector>
#include "InputProvider.h"

class FileInput : public InputProvider {
	// Recorded steps
	std::vector<std::pair<int, char>> steps;
	size_t currentStepIndex = 0;

	// Level settings read from file header
	unsigned int seed = 0;
	unsigned int refreshRateMs = 0;

public:
	FileInput() = default; // Constructor

	// Input playback
	void loadLevelInput(const std::string& levelName) override;
	char getInput(int currentTick) override;

	// Getters
	bool allowEsc() const override { return false; }
	unsigned int getSeed() override { return seed; }
	unsigned int getRefreshRate() const override { return refreshRateMs; }
};