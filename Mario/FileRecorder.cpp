#include "FileRecorder.h"

#include "HelperFunc.h"
#include "Pauline.h"

void FileRecorder::onLevelStart(const std::string& levelName, unsigned int seed, unsigned int refreshRateMs) {
    // 1. Close files from previous level (if any are still open)
    if (stepsFile.is_open())
        stepsFile.close();

    if (resultFile.is_open())
        resultFile.close();

    // 2. Extract base name (e.g. "dkong_01.screen" -> "dkong_01")
    std::string baseName = getBaseName(levelName);
    if (baseName.empty())
        throw std::runtime_error("Invalid level name: " + levelName);
    
    // 3. Open new files for this specific level
    stepsFile.open(baseName + ".steps");
    resultFile.open(baseName + ".result");

    // 4. Record the deterministic seed as the very first line of .steps!
    if (stepsFile.is_open())
        stepsFile << seed << " " << refreshRateMs << "\n";
}

void FileRecorder::onResult(int tick, char eventType, unsigned int expectedScore) {
    if (resultFile.is_open()) {
        resultFile << tick << " " << eventType;
        if (eventType == Pauline::PAULINE_ICON) // Case: Append score only on win
			resultFile << " " << expectedScore;

		resultFile << "\n";
    }
}