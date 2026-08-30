#include <fstream>
#include <filesystem>
#include "ScreenLoader.h"
#include "Board.h"
#include "BigGhost.h"
#include "Mario.h"
#include "SmallGhost.h"
#include "Pauline.h"
#include "DonkeyKong.h"
#include "ExtraLife.h"
#include "Hammer.h"
#include "Tiles.h"

using point = std::pair<int, int>;
// This function returns a sorted list of the names of all the valid screen files found in the game's directory
std::list<std::string> ScreenLoader::getScreenFileNames() {
    namespace fs = std::filesystem;
    std::list<std::string> FilesFromFolder;

    const std::string& directory = "./";
    for (const auto& File : fs::directory_iterator(directory)) {
        std::string filename = File.path().filename().string();
        if (isFileNameValid(filename))
        	FilesFromFolder.push_back(filename);
    }

    FilesFromFolder.sort();

    return FilesFromFolder;
}

// This function receives a screen file's name, and returns a new level built from it, and throws an exception listing the missing or duplicated components if the file is invalid
Level* ScreenLoader::TryLoadLevel(const std::string& fileName) {

	std::ifstream file(fileName);

	if (!file.is_open())
        throw std::runtime_error("Error: Could not open file " + fileName + ".");

    std::vector<std::string> lvl;

    unsigned int marioCount = 0;
	unsigned int paulineCount = 0;
    unsigned int donkeyKongCount = 0;
    unsigned int legendCount = 0;
    bool hasTile = false;
    bool hasWall = false;
    bool hasLadder = false;

	point marioSpawn(-1, -1);
	point paulineSpawn(-1, -1);
	point donkeyKongSpawn(-1, -1);
    point hammerSpawn(-1, -1);
    point legendPosition(-1, -1);

    std::list<point> ghostsSpawns;
    std::list<point> extraLifeSpawns;
    std::string line;
    size_t row = 0;

    while (getline(file, line)) {
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == Mario::MARIO_ICON) { // Found mario
                ++marioCount;
                marioSpawn = std::make_pair(static_cast<int>(i), static_cast<int>(row));
                line[i] = Board::EMPTY;
            }

            if (line[i] == Pauline::PAULINE_ICON) { // Found Pauline
                ++paulineCount;
                paulineSpawn = std::make_pair(static_cast<int>(i), static_cast<int>(row));
            }

            if (line[i] == DonkeyKong::DONKEY_KONG_ICON) { // Found DK
                ++donkeyKongCount;
                donkeyKongSpawn = std::make_pair(static_cast<int>(i), static_cast<int>(row));
            }

            if (line[i] == Hammer::HAMMER_ICON)
                hammerSpawn = std::make_pair(static_cast<int>(i), static_cast<int>(row));

            if (line[i] == 'L') { // Found legend
                ++legendCount;
				legendPosition = std::make_pair(static_cast<int>(i), static_cast<int>(row));
            }

            if (Tiles::isTile(line[i])) // Found at least one tile
                hasTile = true;

            if (line[i] == Board::WALL) // Found at least one wall
                hasWall = true;

            if (line[i] == Board::LADDER) // Found at least one ladder
                hasLadder = true;

            if (line[i] == BigGhost::BIG_GHOST_ICON) 
                ghostsSpawns.emplace_back(static_cast<int>(i), static_cast<int>(row));

            if (line[i] == SmallGhost::SMALL_GHOST_ICON)
                ghostsSpawns.emplace_back(static_cast<int>(i), static_cast<int>(row));

            if (line[i] == ExtraLife::EXTRA_LIFE_ICON) {
                extraLifeSpawns.emplace_back(static_cast<int>(i), static_cast<int>(row));
                line[i] = Board::EMPTY;
            }
        }

        std::string padded_line = line;
        if (padded_line.length() < Board::MAX_X)
            padded_line.append(Board::MAX_X - padded_line.length(), Board::EMPTY);
        
        else if (padded_line.length() > Board::MAX_X) 
            padded_line = padded_line.substr(0, Board::MAX_X); // Cut off extra characters
        
        lvl.emplace_back(padded_line);
        row++;
    }

    // If the file had fewer rows than MAX_Y, fill the rest of the board with empty strings.
    while (row < Board::MAX_Y) {
        lvl.emplace_back(std::string(Board::MAX_X, Board::EMPTY));
        row++;
    }

    if (marioCount != 1 || paulineCount != 1 || donkeyKongCount != 1 || legendCount != 1) {
        std::string errorString;

        if (marioCount != 1) {
	        if (marioCount < 1)
                errorString += std::string("Error: No mario detected.") + "\n";
            else if (marioCount > 1) 
                errorString += std::string("Error: Too many marios detected (Required 1 but detected ") + std::to_string(marioCount) + ")." + "\n";
        }

        if (paulineCount != 1) {
	        if (paulineCount < 1) 
                errorString += std::string("Error: No Pauline detected.") + "\n";
	        else if (paulineCount > 1) 
                errorString += std::string("Error: Too many Paulines detected (Required 1 but detected ") + std::to_string(paulineCount) + ")." + "\n";
        }

		if (donkeyKongCount != 1) {
	        if (donkeyKongCount < 1) 
                errorString += std::string("Error: No Donkey Kong detected.") + "\n";
            else if (donkeyKongCount > 1) 
                errorString += std::string("Error: Too many Donkey Kongs detected (Required 1 but detected ") + std::to_string(donkeyKongCount) + ")." + "\n";
        }

        if (legendCount != 1) {
            if (legendCount < 1) 
                errorString += std::string("Error: No Legend position detected.") + "\n";
            else if (legendCount > 1)
                errorString += "Error: Too many Legend positions detected (Required 1 but detected " + std::to_string(legendCount) + ")." + "\n";
        }

        if (!hasTile) 
            errorString += std::string("Error: No tile detected.") + "\n";

        if (!hasWall)
	        errorString += std::string("Error: No wall detected.") + "\n";

        if (!hasLadder)
            errorString += std::string("Error: No ladder detected.") + "\n";

        errorString.pop_back(); // Remove last new line character
        throw std::invalid_argument(errorString);
    }

    return new Level(fileName, std::move(lvl), marioSpawn, donkeyKongSpawn, paulineSpawn, legendPosition,
        std::move(ghostsSpawns), hammerSpawn, std::move(extraLifeSpawns));
}