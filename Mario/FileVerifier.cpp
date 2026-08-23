#include "FileVerifier.h"
#include <fstream>
#include <stdexcept>
#include <string>

#include "HelperFunc.h"
#include "Pauline.h"

void FileVerifier::onLevelStart(const std::string& levelName, unsigned int seed, unsigned int refreshRateMs) {
    expectedResults.clear();
    currentIndex = 0;

    std::string baseName = getBaseName(levelName);
    if (baseName.empty())
        throw std::runtime_error("Invalid level name: " + levelName);

    std::string resultFileName = baseName + ".result";
    std::ifstream file(resultFileName);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + resultFileName);

    int tick;
    char eventType;

    while (file >> tick >> eventType) {
        if (eventType == Pauline::PAULINE_ICON)
            file >> score;

        expectedResults.emplace_back(tick, eventType);
    }
}

void FileVerifier::onResult(int tick, char actualType, unsigned int expectedScore) {
    if (currentIndex >= expectedResults.size())
        throw std::runtime_error("Test Failed: Unexpected result '" + std::string(1, actualType) + "' at tick " + std::to_string(tick));

    int expectedTick = expectedResults[currentIndex].first;
    char expectedType = expectedResults[currentIndex].second;

    if (tick != expectedTick || actualType != expectedType)
        throw std::runtime_error("Test Failed [Mismatch]: Expected '" + std::string(1, actualType) +
            "' at tick " + std::to_string(tick) + ", but got '" +
            std::string(1, expectedType) + "' at tick " + std::to_string(expectedTick));

    if (actualType == Pauline::PAULINE_ICON && score != expectedScore)
        throw std::runtime_error("Test Failed [Score Mismatch]: Expected score " + std::to_string(expectedScore) +
            ", but got " + std::to_string(score) + " at tick " + std::to_string(tick));

    ++currentIndex;
}