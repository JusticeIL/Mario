#pragma once

using point = std::pair<int, int>;

class Level {
	const std::string filename;
	const std::vector<std::string> level;
	const point marioSpawn;
	const point paulineSpawn;
	const point donkeyKongSpawn;
	const point legendPosition;
	const std::list<point> ghostsSpawnPoints;
	const std::list<point> hammerSpawnPoints;
	const std::list<point> extraLifeSpawnPoints;
public:
	Level(std::string filename, const std::vector<std::string>& lvl, const point &marioSpawn, const point &DKSpawn, const point &paulineSpawn, const point &legendPosition, const std::list<point>& ghostsSpawns, const std::list<point>& hammerSpawns, const std::list<point>& extraLifeSpawns) :
		filename(filename), level(lvl), marioSpawn(marioSpawn), paulineSpawn(paulineSpawn), donkeyKongSpawn(DKSpawn), legendPosition(legendPosition),
		ghostsSpawnPoints(ghostsSpawns), hammerSpawnPoints(hammerSpawns), extraLifeSpawnPoints(extraLifeSpawns) {} // Constructor

	// Getters
	std::vector<std::string> getOriginalLevel() const { return level; }
	int getPaulineSpawnX() const { return paulineSpawn.first; }
	int getPaulineSpawnY() const { return paulineSpawn.second; }
	int getDonkeyKongSpawnX() const { return donkeyKongSpawn.first; }
	int getDonkeyKongSpawnY() const { return donkeyKongSpawn.second; }
	const std::list<std::pair<int, int>>& getGhostsSpawnPoints() const { return ghostsSpawnPoints; }
	const std::list<std::pair<int, int>>& getHammerSpawnPoints() const { return hammerSpawnPoints; }
	const std::list<std::pair<int, int>>& getExtraLifeSpawnPoints() const { return extraLifeSpawnPoints; }
};