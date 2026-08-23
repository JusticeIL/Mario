#include "FileInput.h"
#include <fstream>
#include <stdexcept>

#include "HelperFunc.h"

void FileInput::loadLevelInput(const std::string& levelName) {
    steps.clear();
    currentStepIndex = 0;

    std::string baseName = getBaseName(levelName);
    if (baseName.empty())
        throw std::runtime_error("Invalid level name: " + levelName);

    std::string stepsFileName = baseName + ".steps";
    std::ifstream file(stepsFileName);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + stepsFileName);

    if (!(file >> seed >> refreshRateMs))
        throw std::runtime_error("Failed to read seed and refresh rate");

    int tick;
    char input;
    while (file >> tick >> input)
        steps.emplace_back(tick, input);
}

char FileInput::getInput(int currentTick) {
	if (currentStepIndex < steps.size() && steps[currentStepIndex].first == currentTick)
        return steps[currentStepIndex++].second;

	return '\0';
}