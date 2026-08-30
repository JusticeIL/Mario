#pragma once
#include <vector>
#include <utility>
#include "GameObserver.h"

class FileVerifier : public GameObserver {
	// Expected results loaded from the level's result file
	std::vector<std::pair<int, char>> expectedResults;
	size_t currentIndex = 0;

	// Expected final score
	unsigned int score = 0;

public:
	FileVerifier() = default; // Constructor

	// Verification
	void onLevelStart(const std::string& levelName, unsigned int seed, unsigned int refreshRateMs) override;
	void onStep(int tick, char key) override { return; } // Currently empty implementation because we assume the steps file is always a valid file.
	void onResult(int tick, char actualType, unsigned int expectedScore = 0) override;
};