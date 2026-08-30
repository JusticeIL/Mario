#include "FileInput.h"
#include <fstream>
#include <stdexcept>
#include "HelperFunc.h"

using std::string;

// This function receives the level's name, and loads its steps file, reading the seed, the refresh rate and every recorded tick with the key pressed in it, and throws an exception if the file is missing or invalid
void FileInput::loadLevelInput(const string& levelName) {
    steps.clear();
    currentStepIndex = 0;

    string baseName = getBaseName(levelName);
    if (baseName.empty())
        throw std::runtime_error("Invalid level name: " + levelName);

    string stepsFileName = baseName + ".steps";
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

// This function receives the current tick, and returns the key that was recorded for it, or a null char if no key was recorded for that tick
char FileInput::getInput(int currentTick) {
	if (currentStepIndex < steps.size() && steps[currentStepIndex].first == currentTick)
        return steps[currentStepIndex++].second;

	return '\0';
}