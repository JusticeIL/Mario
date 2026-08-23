#pragma once
#include <fstream>
#include <string>
#include "GameObserver.h"

class FileRecorder : public GameObserver {
	std::ofstream stepsFile;
	std::ofstream resultFile;

public:
    FileRecorder() = default; // Constructor

	~FileRecorder() override { // Destructor
        if (stepsFile.is_open())
            stepsFile.close();
        if (resultFile.is_open())
            resultFile.close();
    }
    void onLevelStart(const std::string& levelName, unsigned int seed, unsigned int refreshRateMs) override;
    void onStep(int tick, char key) override { if (stepsFile.is_open()) stepsFile << tick << " " << key << "\n"; }
    void onResult(int tick, char eventType, unsigned int expectedScore = 0) override;
};