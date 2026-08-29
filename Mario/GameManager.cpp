#include "GameManager.h"
#include <iostream>
#include "Board.h"
#include "BigGhost.h"
#include "SmallGhost.h"
#include "Menu.h"
#include "Mario.h"
#include "Pauline.h"
#include "DonkeyKong.h"
#include "Legend.h"
#include "ConsoleRenderer.h"

// This function frees every object the manager allocated: the levels list, the uncollected extra lives, Mario, Pauline, Donkey Kong, the hammer and the legend
GameManager::~GameManager() { // Destructor
	// Cleaning the levels list before exiting the game to avoid memory leaks
	for (auto levelIterator = levels.begin(); levelIterator != levels.end(); ) {
		delete* levelIterator;                          // 1. Free the allocated memory
		levelIterator = levels.erase(levelIterator); // 2. Remove pointer from list and get next iterator
	}

	for (auto* life : uncollectedExtraLives)
		delete life;

	delete mario;
	delete pauline;
	delete donkeyKong;

	delete uncollectedHammer;
	delete legend;
}

// This function runs the level's main loop, handling input, enemy movement, collisions, scoring and the win condition on every tick, and returns Won, Lost or Paused according to how the level ended
GameManager::GameResult GameManager::playGame() {
	if (renderer)
		renderer->renderBoard(board, *legend, isColor);

	while (true) {
		// Handle user input
		char key = inputProvider->getInput(ticks);

		if (key != '\0') {
			if (inputProvider->allowEsc() && key == Menu::ESC_CH)
				return GameResult::Paused;

			mario->setPressedKey(key);

			if (observer)
				observer->onStep(ticks, key);
		}

		// Enemies movement
		if (ticks % 2 == 0)
			updateBarrels();

		if (ticks % 4 == 0)
			updateGhosts();

		if (ticks % 100 == 0)
			donkeyKongThrowsNewBarrel();

		// Enemies collision
		for (auto& barrel : barrels)
			barrel.updateIfHitMario(mario->getMarioX(), mario->getMarioY());

		for (auto& ghost : ghosts)
			ghost->updateIfHitMario(mario->getMarioX(), mario->getMarioY());

		donkeyKong->updateIfHitMario(mario->getMarioX(), mario->getMarioY());

		// Easter egg
		if (mario->isJumping() && legend != nullptr && legend->isScoreCoordinate(mario->getMarioX(), mario->getMarioY() - 1)) {
			mario->bumpHead();
			triggerLegendBump();
		}

		mario->move();
		mario->tryPickUpHammer(uncollectedHammer);

		if (mario->hasHammer() && mario->getHammer()->isSwinging()) {
			Hammer* h = mario->getHammer();

			for (int i = 0; i < 2; i++) {
				int smashX = h->getHitX(i);
				int smashY = h->getHitY();

				// 1. Check Barrels
				for (auto& barrel : barrels)
					if (!barrel.amIDead() && barrel.getX() == smashX && barrel.getY() == smashY) { // If barrel is dead we don't want to grant additional 10 points
						barrel.killed();
						addScore(10);
					}

				// 2. Check Ghosts
				for (auto& ghost : ghosts) 
					if (!ghost->amIDead() && ghost->getX() == smashX && ghost->getY() == smashY) { // If ghost is dead we don't want to grant additional 10 points
						ghost->killed();
						addScore(10);
					}
			}
		}

		tryCollectExtraLives();

		pauline->updateWinCondition(mario->getMarioX(), mario->getMarioY());
		if (checkWinCondition()) {
			if (observer)
				observer->onResult(ticks, Pauline::PAULINE_ICON, score);

			currentLevel++;
			if (singleLevelMode || currentLevel == levels.end())  // Case: They beat the whole game!
				return GameResult::Won;

			prepareLevelData();

			for (auto* life : uncollectedExtraLives)
				life->drawToBoard();

			if (renderer) {
				renderer->clear();
				renderer->renderBoard(board, *legend, isColor);
			}

			ticks++;
			continue; // Skips the rest of the tick since we just loaded a new level
		}

		auto report = checkIfMarioHit();
		if (report.source != DamageSource::None) {
			char damageChar = '\0';

			if (report.sourceEntity)
				damageChar = report.sourceEntity->getIcon();
			else // Case: fall damage
				damageChar = Mario::MARIO_ICON;

			if (observer && damageChar != '\0')
				observer->onResult(ticks, damageChar);

			handleMarioDeath();
			if (mario->marioLifePoints() <= 0)
				return GameResult::Lost;
		}

		if (renderer)
			renderer->wait(renderDelayMs);

		++ticks;
		manageScore();
	}
}

// This function moves every barrel one step and removes from the list the barrels that are dead
void GameManager::updateBarrels() {
	for (auto barrelIterator = barrels.begin(); barrelIterator != barrels.end();) {
		barrelIterator->move();

		if (barrelIterator->isExploded())
			barrelIterator = barrels.erase(barrelIterator);
		else
			++barrelIterator;
	}
}

// This function moves every ghost one step and removes from the list the ghosts that are dead
void GameManager::updateGhosts() {
	for (auto ghostIterator = ghosts.begin(); ghostIterator != ghosts.end();) {
		(*ghostIterator)->move();

		if ((*ghostIterator)->amIDead())
			ghostIterator = ghosts.erase(ghostIterator);
		else
			++ghostIterator;
	}
}

// This function creates the uncollected hammer of the level, receives level as the loaded level holding its spawn point, and skips creation if the level has no hammer or one already exists
void GameManager::initializeHammer(const Level& level) {
	int hammerXPos = level.getHammerSpawnX();
	int hammerYPos = level.getHammerSpawnY();
	if (uncollectedHammer == nullptr && hammerXPos != -1 && hammerYPos != -1)
		uncollectedHammer = new Hammer(hammerXPos, hammerYPos, board, isColor);
}

// This function creates the extra lives of the level and draws them on the board, receives level as the loaded level holding their spawn points
void GameManager::readExtraLives(const Level& level) {
	for (const auto& spawn : level.getExtraLifeSpawnPoints()) {
		ExtraLife* life = new ExtraLife(spawn.first, spawn.second, board, isColor);
		uncollectedExtraLives.push_back(life);
		life->drawToBoard();
	}
}

// This function grants Mario an extra life if he stepped on one, deletes the collected item and refreshes the legend
void GameManager::tryCollectExtraLives() {
	bool lifeGained = false;
	for (auto extraLifeIterator = uncollectedExtraLives.begin(); extraLifeIterator != uncollectedExtraLives.end(); ) {
		if ((*extraLifeIterator)->checkIfMarioPickedUpLife(mario->getMarioX(), mario->getMarioY())) {
			mario->pickUpLife();

			delete* extraLifeIterator; // Destructor automatically erases it from the board
			extraLifeIterator = uncollectedExtraLives.erase(extraLifeIterator);

			lifeGained = true;
			break; // Only one life can be collected at a board step
		}
		else
			++extraLifeIterator;
	}

	if (renderer &&lifeGained)
		legend->drawToConsole();
}

// This function deletes all the entities of the current level and empties the barrels and ghosts containers
void GameManager::clearAllEntities() {
	delete donkeyKong;
	donkeyKong = nullptr;

	delete pauline;
	pauline = nullptr;

	delete uncollectedHammer;
	uncollectedHammer = nullptr;

	delete legend;
	legend = nullptr;

	for (auto* life : uncollectedExtraLives)
		delete life;
	uncollectedExtraLives.clear();

	barrels.clear();
	ghosts.clear();
}

// This function lowers the score by one every fixed amount of ticks, calculated from the refresh rate, and refreshes the legend
void GameManager::manageScore() {
	int tickThreshold = static_cast<int>(logicalRefreshRateMs * -0.35 + 57.5);
	if (tickThreshold < 1) // Case: refreshRateMs is too slow to calculate a proper threshold
		tickThreshold = 1;

	if (ticks > 0 && ticks % tickThreshold == 0 && score > 0) {
		score--;
		if (renderer)
			legend->drawToConsole(); // Update UI instantly
	}
}

// This function receives points, adds them to the score and refreshes the legend
void GameManager::addScore(int points) {
	score += points;
	if (renderer)
		legend->drawToConsole(); // Update UI instantly
}

// This function rewards Mario for bumping his head into the legend with 10 points, an occasional extra life dropped next to it and a yellow flash if in color mode
void GameManager::triggerLegendBump() { /* Easter Egg */
	// 1. Reward points
	score += 10;

	// 2. Extra Life Drop (OCCASIONAL)
	if (score % 7 == 0) {
		int rewardX = board.getLevel().getLegendPositionX() - 2;
		int rewardY = board.getLevel().getLegendPositionY() + 3;

		if (board.isWithinBounds(rewardX, rewardY) && board.getBoardChar(rewardX, rewardY) == Board::EMPTY) {
			ExtraLife* dynamicLife = new ExtraLife(rewardX, rewardY, board, isColor);
			dynamicLife->drawToBoard();
			dynamicLife->drawToConsole();
			uncollectedExtraLives.push_back(dynamicLife);
		}
	}

	// 3. Color Bump Flash Effect
	if (renderer)
		legend->flashYellow();
}

// This function clears the barrels, respawns the ghosts of the level with the current seed and resets Donkey Kong
void GameManager::resetEnemies() {
	barrels.clear();
	ghosts.clear();
	readGhosts(board.getLevel(), currentLevelSeed);
	donkeyKong->reset();
}

// This function receives the loaded level holding the spawn points and seed, and creates the ghosts of the level according to their icon on the board
void GameManager::readGhosts(const Level& level, unsigned int seed) {
	const auto& lvl = level.getOriginalLevel();
	const auto& ghostSpawns = level.getGhostsSpawnPoints();

	for (const auto& spawn : ghostSpawns) {
		
		char icon = lvl[spawn.second][spawn.first];

		if (icon == SmallGhost::SMALL_GHOST_ICON)
			ghosts.push_back(std::make_unique<SmallGhost>(spawn.first, spawn.second, board, isColor, seed));
		else if (icon == BigGhost::BIG_GHOST_ICON)
			ghosts.push_back(std::make_unique<BigGhost>(spawn.first, spawn.second, board, isColor, mario->getMarioXRef(), mario->getMarioYRef()));
	}
}

// This function receives the loaded level holding the spawn point and creates Mario on the first setup or only updates his spawn point on the following levels
void GameManager::initializeMario(const Level& level) {
	if (mario == nullptr) // Case: first time setup
		mario = new Mario(level.getMarioSpawnX(), level.getMarioSpawnY(), board, isColor);
	else // Case: next level setup
		mario->setSpawnPoint(level.getMarioSpawnX(), level.getMarioSpawnY());
}

// This function receives the loaded level holding Donkey Kong's spawn point and creates him if he hasn't been created yet
void GameManager::initializeDonkeyKong(const Level& level) {
	if (donkeyKong == nullptr)
		donkeyKong = new DonkeyKong(level.getDonkeyKongSpawnX(), level.getDonkeyKongSpawnY(), board, isColor);
}

// This function receives the loaded level holding Pauline's spawn point and creates her if she hasn't been created yet
void GameManager::initializePauline(const Level& level) {
	if (pauline == nullptr)
		pauline = new Pauline(level.getPaulineSpawnX(), level.getPaulineSpawnY(), board, isColor);
}

// This function returns a damage report holding the damage source (barrel, ghost, Donkey Kong, fall or None) and a pointer to the entity that caused it
GameManager::MarioDamageReport GameManager::checkIfMarioHit() const {
	for (const auto& barrel : barrels) // Barrels
		if (barrel.isHitMario())
			return MarioDamageReport{ &barrel, DamageSource::Barrel };

	for (const auto& ghost : ghosts) // Ghosts
		if (ghost->isHitMario())
			return MarioDamageReport{ ghost.get(), DamageSource::Ghost };

	if (donkeyKong->isHitMario()) // Donkey Kong
		return MarioDamageReport{ donkeyKong, DamageSource::DonkeyKong };

	if (mario->amIDead()) // Mario is hit or dead from fall damage
		return MarioDamageReport{ nullptr, DamageSource::Fall };

	return MarioDamageReport{ nullptr, DamageSource::None };
}

// This function takes one life from Mario and restarts the level, resetting Mario, the board, the enemies and the hammer, and redrawing the screen
void GameManager::handleMarioDeath() {
	std::cin.clear();

	if (mario->marioLifePoints() > 0)
		mario->decreaseLife();

	mario->reset();
	board.reset();

	if (renderer)
		legend->drawToConsole();

	resetEnemies();

	delete uncollectedHammer;
	uncollectedHammer = nullptr;
	initializeHammer(board.getLevel());

	for (auto* life : uncollectedExtraLives)
		life->drawToBoard();

	if (renderer)
		renderer->clear();

	if (renderer)
		renderer->renderBoard(board, *legend, isColor);
}

// This function returns true if Mario reached Pauline and false otherwise
bool GameManager::checkWinCondition() const {
	return pauline->checkWinCondition();
}

// This function starts a full game run from the first level and restores Mario's lives, and does nothing if no levels were loaded
void GameManager::startNewGame() {
	if (levels.empty()) // Case: no levels are loaded at all
		return;

	singleLevelMode = false;

	// Reset progress to the first level
	currentLevel = levels.begin();

	if (mario != nullptr)
		mario->restoreLives();

	prepareLevelData();
}

// This function loads the current level into the board, reads its input data (seed and refresh rate), builds all its entities and resets Mario for the new screen
void GameManager::prepareLevelData() {
	// 1. Clear out any old state
	ticks = 0;
	clearAllEntities();

	// 2. Load the target level
	board.setLevel(*currentLevel);

	// Load the level input data from the input provider
	std::string currentFilename = (*currentLevel)->getFilename();
	inputProvider->loadLevelInput(currentFilename);
	currentLevelSeed = inputProvider->getSeed();
	unsigned int fileMs = inputProvider->getRefreshRate();
	if (fileMs > 0) {
		logicalRefreshRateMs = fileMs; // Use the file's original difficulty for math
		renderDelayMs = 5;             // Fast-forward the visual render speed!
	}

	if (observer)
		observer->onLevelStart(currentFilename, currentLevelSeed, logicalRefreshRateMs);

	// Setup DK, ghosts, barrels, hammer and extra lives for the new board
	setupNewLevel();

	// 3. Reset Mario and safely redraw
	if (mario != nullptr) {
		mario->reset();
		board.reset();
	}

	for (auto* life : uncollectedExtraLives)
		life->drawToBoard();
}

// This function receives the name of the screen file to play, starts a single level chosen from the menu by name and restores Mario's lives before loading it
void GameManager::startSpecificLevel(const std::string& filename) {
	if (levels.empty()) // Case: no levels are loaded at all
		return;

	singleLevelMode = true;

	// Find the requested level by name
	for (auto it = levels.begin(); it != levels.end(); ++it) 
		if ((*it)->getFilename() == filename) {
			currentLevel = it;
			break;
		}

	if (mario != nullptr)
		mario->restoreLives();

	prepareLevelData();
}

// This function builds all the entities of the current board and creates a new legend with a full score
void GameManager::setupNewLevel() {
	initializeDonkeyKong(board.getLevel());
	initializeMario(board.getLevel());
	initializePauline(board.getLevel());
	initializeHammer(board.getLevel());

	readExtraLives(board.getLevel());
	readGhosts(board.getLevel(), currentLevelSeed);

	score = MAX_INIT_SCORE;
	legend = new Legend(board.getLevel().getLegendPositionX(), board.getLevel().getLegendPositionY(), mario->getMarioLifeRef(), score, isColor);
}

// This function adds a new barrel created by Donkey Kong to the barrels list
void GameManager::donkeyKongThrowsNewBarrel() {
	barrels.push_back(*donkeyKong->createBarrel());
}

// This function loads every valid screen file into the levels list, and stores in the error log the error message of each file that failed to load
void GameManager::loadAllScreens() {
	for (const auto& filename : screenLoader.getScreenFileNames()) {
		try {
			levels.push_back(screenLoader.TryLoadLevel(filename));
		}
		catch (const std::exception& e) {
			error_log[filename] = e.what();
		}
	}
}