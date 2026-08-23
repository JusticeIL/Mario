#pragma once
#include <string>

class GameObserver {
public:
    virtual ~GameObserver() = default;
    virtual void onLevelStart(const std::string& levelName, unsigned int seed, unsigned int refreshRateMs) = 0;
    virtual void onStep(int tick, char key) = 0;
    virtual void onResult(int tick, char eventType, unsigned int expectedScore = 0) = 0;
};