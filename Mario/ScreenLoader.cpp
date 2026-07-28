#include <fstream>
#include <filesystem>
#include "ScreenLoader.h"

#include "BigGhost.h"
#include "Mario.h"
#include "SmallGhost.h"

using point = std::pair<int, int>;

// This function retrieves the names of valid screen files in the current directory
std::list<std::string> ScreenLoader::getScreenFileNames() {
    namespace fs = std::filesystem;
    std::list<std::string> FilesFromFolder;

    const std::string& directory = "./";
    for (const auto& File : fs::directory_iterator(directory)) {
        std::string filename = File.path().filename().string();
        if (isFileNameValid(filename))
        	FilesFromFolder.push_back(filename);
    }

    return FilesFromFolder;
}

Level ScreenLoader::TryLoadLevel(const string fileName) {

	std::ifstream file(fileName);

	if (!file.is_open())
		return nullptr;

    std::vector<std::string> lvl;

    unsigned int marioCount = 0;
	unsigned int paulineCount = 0;
    unsigned int donkeyKongCount = 0;
    unsigned int legendCount = 0;

	point marioSpawn(-1, -1);
	point paulineSpawn(-1, -1);
	point donkeyKongSpawn(-1, -1);
    point legendPosition(-1, -1);

    std::list<point>& ghostsSpawns;
    std::list<point>& hammerSpawns;
    std::list<point>& extraLifeSpawns;
    std::string line;
    size_t row = 0;

    while (getline(file, line)) {
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == Mario::MARIO_ICON) { // Find mario
                ++marioCount;
                marioSpawn = std::make_pair(i, row);
            }

            if (line[i] == Pauline::PAULINE_ICON) { // Find Pauline
                ++paulineCount;
                paulineSpawn = std::make_pair(i, row);
            }

            if (line[i] == DonkeyKong::DONKEY_KONG_ICON) { // Find DK
                ++donkeyKongCount;
                donkeyKongSpawn = std::make_pair(i, row);
            }

            if (line[i] == 'L') // Find legend
                ++legendCount;

            if (line[i] == BigGhost::BIG_GHOST_ICON)
                ghostsSpawns.emplace_back(i, row);

            if (line[i] == SmallGhost::SMALL_GHOST_ICON)
                ghostsSpawns.emplace_back(i, row);

            if (line[i] == Hammer::HAMMER_ICON)
                hammerSpawns.emplace_back(i, row);

            /* TODO: if (line[i] == ExtraLife::EXTRA_LIFE_ICON)
                extraLifeSpawns.emplace_back(i, row); */
        }

        lvl.emplace_back(line);
        row++;
    }

    if (marioCount != 1 || paulineCount != 1 || donkeyKongCount != 1 || legendCount != 1) {
        string errorString;

        if (marioCount != 1) {
	        if (marioCount < 1) {
                errorString += "Error: No mario detected." + "\n";
            } else if (marioCount > 1) {
                errorString += "Error: Too many marios detected (Required 1 but detected " + std::to_string(marioCount) + ")." + "\n";
			}
        }

        if (paulineCount != 1) {
	        if (paulineCount < 1) {
                errorString += "Error: No Pauline detected." + "\n";
            } else if (paulineCount > 1) {
                errorString += "Error: Too many Paulines detected (Required 1 but detected " + std::to_string(paulineCount) + ")." + "\n";
			}
        }

		if (donkeyKongCount != 1) {
	        if (donkeyKongCount < 1) {
                errorString += "Error: No Donkey Kong detected." + "\n";
            } else if (donkeyKongCount > 1) {
                errorString += "Error: Too many Donkey Kongs detected (Required 1 but detected " + std::to_string(donkeyKongCount) + ")." + "\n";
			}
        }

        if (legendCount != 1) {
            if (legendCount < 1) {
                errorString += "Error: No Legend position detected." + "\n";
            }
            else if (legendCount > 1) {
                errorString += "Error: Too many Legend positions detected (Required 1 but detected " + std::to_string(legendCount) + ")." + "\n";
            }
        }

        errorString.pop_back();
        throw std::invalid_argument(errorString);
    }

	return Level(fileName, lvl, marioSpawn, donkeyKongSpawn, paulineSpawn, legendPosition, ghostsSpawns, hammerSpawns, extraLifeSpawns);
}
