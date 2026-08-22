#pragma once
#include <list>
#include <string>
#include <vector>

using point = std::pair<int, int>;

class Level {
	const std::string filename;
	const std::vector<std::string> level;
	const point marioSpawn;
	const point paulineSpawn;
	const point donkeyKongSpawn;
	const point legendPosition;
	const std::list<point> ghostsSpawnPoints;
	const point hammerSpawnPoint;
	const std::list<point> extraLifeSpawnPoints;
public:
	Level(std::string filename, const std::vector<std::string>& lvl, const point &marioSpawn, const point &DKSpawn, const point &paulineSpawn, const point &legendPosition, const std::list<point>& ghostsSpawns, const point &hammerSpawn, const std::list<point>& extraLifeSpawns) :
		filename(filename), level(lvl), marioSpawn(marioSpawn), paulineSpawn(paulineSpawn), donkeyKongSpawn(DKSpawn), legendPosition(legendPosition),
		ghostsSpawnPoints(ghostsSpawns), hammerSpawnPoint(hammerSpawn), extraLifeSpawnPoints(extraLifeSpawns) {} // Constructor

	// Getters
	const std::string& getFilename() const { return filename; }
	const std::vector<std::string>& getOriginalLevel() const { return level; }
	int getMarioSpawnX() const { return marioSpawn.first; }
	int getMarioSpawnY() const { return marioSpawn.second; }
	int getPaulineSpawnX() const { return paulineSpawn.first; }
	int getPaulineSpawnY() const { return paulineSpawn.second; }
	int getDonkeyKongSpawnX() const { return donkeyKongSpawn.first; }
	int getDonkeyKongSpawnY() const { return donkeyKongSpawn.second; }
	int getHammerSpawnX() const { return hammerSpawnPoint.first; }
	int getHammerSpawnY() const { return hammerSpawnPoint.second; }
	int getLegendPositionX() const { return legendPosition.first; }
	int getLegendPositionY() const { return legendPosition.second; }
	const std::list<std::pair<int, int>>& getGhostsSpawnPoints() const { return ghostsSpawnPoints; }
	const std::list<std::pair<int, int>>& getExtraLifeSpawnPoints() const { return extraLifeSpawnPoints; }
};