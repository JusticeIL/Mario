#pragma once
#include <string>

class InputProvider {
public:
	virtual ~InputProvider() = default;
	virtual void loadLevelInput(const std::string& levelName) = 0;
	virtual char getInput(int currentTick) = 0;
	virtual bool allowEsc() const = 0;
	virtual unsigned int getSeed() = 0;
	virtual unsigned int getRefreshRate() const = 0;
};