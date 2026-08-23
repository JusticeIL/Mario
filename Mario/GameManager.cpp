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

// This function initiates the gameplay, manages the snake movement, collision checks, and apple interactions
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

void GameManager::updateBarrels() {
	for (auto barrelIterator = barrels.begin(); barrelIterator != barrels.end();) {
		barrelIterator->move();

		if (barrelIterator->isExploded())
			barrelIterator = barrels.erase(barrelIterator);
		else
			++barrelIterator;
	}
}

void GameManager::updateGhosts() {
	for (auto ghostIterator = ghosts.begin(); ghostIterator != ghosts.end();) {
		(*ghostIterator)->move();

		if ((*ghostIterator)->amIDead())
			ghostIterator = ghosts.erase(ghostIterator);
		else
			++ghostIterator;
	}
}

void GameManager::initializeHammer(const Level& level) {
	int hammerXPos = level.getHammerSpawnX();
	int hammerYPos = level.getHammerSpawnY();
	if (uncollectedHammer == nullptr && hammerXPos != -1 && hammerYPos != -1)
		uncollectedHammer = new Hammer(hammerXPos, hammerYPos, board, isColor);
}

void GameManager::readExtraLives(const Level& level) {
	for (const auto& spawn : level.getExtraLifeSpawnPoints()) {
		ExtraLife* life = new ExtraLife(spawn.first, spawn.second, board, isColor);
		uncollectedExtraLives.push_back(life);
		life->drawToBoard();
	}
}

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

void GameManager::addScore(int points) {
	score += points;
	if (renderer)
		legend->drawToConsole(); // Update UI instantly
}

void GameManager::triggerLegendBump() {
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

void GameManager::resetEnemies() {
	barrels.clear();
	ghosts.clear();
	readGhosts(board.getLevel(), currentLevelSeed);
	donkeyKong->reset();
}

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

void GameManager::initializeMario(const Level& level) {
	if (mario == nullptr) // Case: first time setup
		mario = new Mario(level.getMarioSpawnX(), level.getMarioSpawnY(), board, isColor);
	else // Case: next level setup
		mario->setSpawnPoint(level.getMarioSpawnX(), level.getMarioSpawnY());
}

void GameManager::initializeDonkeyKong(const Level& level) {
	if (donkeyKong == nullptr)
		donkeyKong = new DonkeyKong(level.getDonkeyKongSpawnX(), level.getDonkeyKongSpawnY(), board, isColor);
}

void GameManager::initializePauline(const Level& level) {
	if (pauline == nullptr)
		pauline = new Pauline(level.getPaulineSpawnX(), level.getPaulineSpawnY(), board, isColor);
}

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

bool GameManager::checkWinCondition() const {
	return pauline->checkWinCondition();
}

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

void GameManager::donkeyKongThrowsNewBarrel() {
	barrels.push_back(*donkeyKong->createBarrel());
}

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