#pragma once
#include <list>
#include <string>
#include <vector>
#include <utility>

using point = std::pair<int, int>;

class Level {
	// Level identity and layout
	const std::string filename;
	const std::vector<std::string> level;

	// Spawn points
	const point marioSpawn;
	const point paulineSpawn;
	const point donkeyKongSpawn;
	const std::list<point> ghostsSpawnPoints;
	const point hammerSpawnPoint;
	const std::list<point> extraLifeSpawnPoints;

	// Legend position
	const point legendPosition;
public:
	Level(std::string filename, std::vector<std::string> lvl, const point& marioSpawn, const point& DKSpawn, const point& paulineSpawn, const point& legendPosition, std::list<point> ghostsSpawns, const point& hammerSpawn, std::list<point> extraLifeSpawns) :
		filename(std::move(filename)), level(std::move(lvl)), marioSpawn(marioSpawn), paulineSpawn(paulineSpawn), donkeyKongSpawn(DKSpawn), ghostsSpawnPoints(std::move(ghostsSpawns)),
		hammerSpawnPoint(hammerSpawn), extraLifeSpawnPoints(std::move(extraLifeSpawns)), legendPosition(legendPosition) {} // Constructor

	// Level identity and layout
	const std::string& getFilename() const { return filename; }
	const std::vector<std::string>& getOriginalLevel() const { return level; }

	// Legend position
	int getLegendPositionX() const { return legendPosition.first; }
	int getLegendPositionY() const { return legendPosition.second; }

	// Getters
	int getMarioSpawnX() const { return marioSpawn.first; }
	int getMarioSpawnY() const { return marioSpawn.second; }
	int getPaulineSpawnX() const { return paulineSpawn.first; }
	int getPaulineSpawnY() const { return paulineSpawn.second; }
	int getDonkeyKongSpawnX() const { return donkeyKongSpawn.first; }
	int getDonkeyKongSpawnY() const { return donkeyKongSpawn.second; }
	int getHammerSpawnX() const { return hammerSpawnPoint.first; }
	int getHammerSpawnY() const { return hammerSpawnPoint.second; }
	const std::list<std::pair<int, int>>& getGhostsSpawnPoints() const { return ghostsSpawnPoints; }
	const std::list<std::pair<int, int>>& getExtraLifeSpawnPoints() const { return extraLifeSpawnPoints; }
};