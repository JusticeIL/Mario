#pragma once
#include <string>

class InputProvider {
public:
	virtual ~InputProvider() = default; // Destructor

	// Input playback
	virtual void loadLevelInput(const std::string& levelName) = 0;
	virtual char getInput(int currentTick) = 0;

	// Getters
	virtual bool allowEsc() const = 0;
	virtual unsigned int getSeed() = 0;
	virtual unsigned int getRefreshRate() const = 0;
};