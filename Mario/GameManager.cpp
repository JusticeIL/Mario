#include "GameManager.h"

#include <conio.h>
#include <random>
#include <windows.h>
#include "Board.h"
#include "BigGhost.h"
#include "SmallGhost.h"
#include "Menu.h"
#include "Mario.h"
#include "Pauline.h"
#include "DonkeyKong.h"

GameManager::~GameManager() { // Destructor
	// Cleaning the levels list before exiting the game to avoid memory leaks
	for (auto levelIterator = levels.begin(); levelIterator != levels.end(); ) {
		delete* levelIterator;                          // 1. Free the allocated memory
		levelIterator = levels.erase(levelIterator); // 2. Remove pointer from list and get next iterator
	}

	delete mario;
	delete pauline;
	delete donkeyKong;
}

// This function initiates the gameplay, manages the snake movement, collision checks, and apple interactions
GameManager::GameResult GameManager::playGame() {
	static int ticks = 0;
	board.print();

	while (true) {
		// Handle user input
		if (_kbhit()) {
			char key = _getch();

			if (key == Menu::ESC_CH)
				return GameResult::Paused;

			mario->setPressedKey(tolower(key));
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

		mario->move();
		mario->tryPickUpHammer(uncollectedHammer);

		if (mario->hasHammer() && mario->getHammer()->isSwinging()) {
			Hammer* h = mario->getHammer();

			for (int i = 0; i < 2; i++) {
				int smashX = h->getHitX(i);
				int smashY = h->getHitY();

				// 1. Check Barrels
				for (auto& barrel : barrels)
					if (barrel.getX() == smashX && barrel.getY() == smashY) 
						barrel.killed();

				// 2. Check Ghosts
				for (auto& ghost : ghosts) 
					if (ghost->getX() == smashX && ghost->getY() == smashY)
						ghost->killed();
			}
		}

		pauline->updateWinCondition(mario->getMarioX(), mario->getMarioY());
		if (checkWinCondition()) {
			currentLevel++;
			if (currentLevel == levels.end())  // Case: They beat the whole game!
				return GameResult::Won;

			clearAllEntities();
			board.setLevel(*currentLevel);
			setupNewLevel();
			mario->reset();
			clearScr();
			board.print();
			ticks++;
			continue; // Skips the rest of the tick since we just loaded a new level
		}

		auto report = checkIfMarioHit();
		if (report.source != DamageSource::None)
			handleMarioDeath();

		if (mario->marioLifePoints() <= 0)
			return GameResult::Lost;
		
		Sleep(difficultyLevel == Difficulty::Hard ? 50 : 150);
		ticks++;
	}
}

// This function generates a random seed for the small ghost
unsigned int GameManager::randomizeSeedForSmallGhost() {

	std::random_device rd;
	unsigned int seedRes = rd();

	return seedRes;
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

void GameManager::clearAllEntities() {
	delete donkeyKong;
	donkeyKong = nullptr;

	delete mario;
	mario = nullptr;

	delete pauline;
	pauline = nullptr;

	delete uncollectedHammer;
	uncollectedHammer = nullptr;

	barrels.clear();
	ghosts.clear();
}

void GameManager::resetEnemies() {
	barrels.clear();
	ghosts.clear();
	readGhosts(board.getLevel());
}

void GameManager::readGhosts(const Level& level) {
	const auto& lvl = level.getOriginalLevel();
	const auto& ghostSpawns = level.getGhostsSpawnPoints();
	unsigned int seedForSmallGhost = randomizeSeedForSmallGhost();

	for (const auto& spawn : ghostSpawns) {
		
		char icon = lvl[spawn.second][spawn.first];

		if (icon == SmallGhost::SMALL_GHOST_ICON)
			ghosts.push_back(std::make_unique<SmallGhost>(spawn.first, spawn.second, board, isColor, seedForSmallGhost));
		else if (icon == BigGhost::BIG_GHOST_ICON)
			ghosts.push_back(std::make_unique<BigGhost>(spawn.first, spawn.second, board, isColor, mario->getMarioXRef(), mario->getMarioYRef()));
	}
}

void GameManager::initializeMario(const Level& level) {
	if (mario == nullptr)
		mario = new Mario(level.getMarioSpawnX(), level.getMarioSpawnY(), board, isColor);
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
	// TODO: reset logic after mario death
	std::cin.clear();

	if (mario->marioLifePoints() > 0)
		mario->decreaseLife();

	mario->reset();
	board.reset(mario->marioLifePoints());
	resetEnemies();

	delete uncollectedHammer;
	uncollectedHammer = nullptr;
	initializeHammer(board.getLevel());

	clearScr();
	board.print();
}

bool GameManager::checkWinCondition() const {
	return pauline->checkWinCondition();
}

void GameManager::startNewGame() {
	if (levels.empty()) // Case: no levels are loaded at all
		return;

	clearAllEntities();

	// Reset progress to the first level
	currentLevel = levels.begin();
	board.setLevel(*currentLevel);

	// Setup DK, ghosts, and barrels for the new board
	setupNewLevel();

	if (mario != nullptr) {
		mario->restoreLives();
		mario->reset();
		board.reset(mario->marioLifePoints());
	}
}

void GameManager::setupNewLevel() {
	initializeDonkeyKong(board.getLevel());
	initializeMario(board.getLevel());
	initializePauline(board.getLevel());
	initializeHammer(board.getLevel());

	readGhosts(board.getLevel());
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
