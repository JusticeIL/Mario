#include "GameManager.h"

#include <conio.h>
#include <random>
#include <windows.h>

#include "BigGhost.h"
#include "SmallGhost.h"

// This function manages the main game loop, handles transitions between different game states, and coordinates user inputs and game actions
void GameManager::run() {

	menu.printMainMenu();

	while (state != GameState::Exit) { // Game loop runs as long as ESC key wasn't pressed
		if (state == GameState::Standby) {
			choiceHolder = menu.handleMenu();
			switch (choiceHolder) {
			case PLAY: // Start game
				state = GameState::Playing;
				choiceHolder = '\0';
				menu.ResetMenu();
				menu.resetAllArrows();
				firstPrint = true;
				break;
			case INSTRUCTIONS: // Instructions
				state = GameState::Instructions;
				choiceHolder = '\0';
				menu.ResetMenu();
				menu.resetAllArrows();
				firstPrint = true;
				break;
			case OPTIONS: // Options
				state = GameState::Options;
				choiceHolder = '\0';
				menu.ResetMenu();
				menu.resetAllArrows();
				firstPrint = true;
				break;
			case QUIT: // Exit
				clearScr();
				state = GameState::Exit;
				break;
			}
		}

		if (state == GameState::Playing)
			playGame();
		if (state == GameState::Pause)	{
			printPauseScreen();
			paused = true;
			if (_kbhit()) {
				char pressedKey = _getch();
				if (pressedKey == ESC) { // Unpause only after ESC
					terminatePause();
					state = GameState::Playing;
				}
			}
		}
		else if (state == GameState::GameOver)
			gameOverLogic();

		else if (state == GameState::GameWon)
			gameWonLogic();

		if (state != GameState::Playing && state != GameState::Standby && state != GameState::Pause) { // Case: menu screens
			printScreens();
			handleState();
		}
	}
}

void GameManager::printScreens() {

	switch (state) {
	case GameState::Instructions:
	case GameState::Options:
		menu.ChangeMenuChar('\0');
		break;
	}

	if (firstPrint)
	{
		clearScr(); // Clear the console
		gotoxy(0, 0);

		switch (state)
		{
		case GameState::Instructions:
			menu.printInstructionsScreen();
			break;

		case GameState::Options:
			menu.printOptionsScreen();
			// Difficulty printing
			gotoxy(56, 6);
			if (difficultyLevel == Difficulty::Easy)
				std::cout << "Easy";
			else // Case: difficultylevel == HARD
				std::cout << "Hard";
			break;

			firstPrint = false;
		}
	}
}

// This function processes user inputs and updates the game state accordingly during non-gameplay states
void GameManager::handleState() {

	static DWORD messageTimestamp = 0;
	static bool messageOnScreen = false;

	if (_kbhit()) {
		char pressedKey = _getch();
		pressedKey = tolower(pressedKey);
		menu.ChangeMenuChar(pressedKey);

		switch (state)
		{
		case GameState::Instructions:
			if (pressedKey == ESC) {// Case: returning to main menu
				menu.ResetMenu(); // Resetting menu variables
				firstPrint = true;
				clearScr(); // Clear the screen before returning to the menu
				menu.printMainMenu(); // Display menu again
				state = GameState::Standby;
				menu.resetAllArrows();
			}
			break;
		case GameState::Options:
			if (pressedKey == '5') {
				isColor = !isColor; // Toggle color mode
				gotoxy(56, 5);
				std::cout << (isColor ? "ON " : "OFF");
				clearScr(); // Clear the screen
				firstPrint = true;
			}
			else if (pressedKey == '7')	{
				difficultyLevel = difficultyLevel == Difficulty::Easy ? Difficulty::Hard : Difficulty::Easy; // Toggle difficulty level
				gotoxy(60, 7);
				gotoxy(60, 7);
				std::cout << std::string(10, ' '); // Clear message
				gotoxy(60, 7);
				std::cout << (difficultyLevel == Difficulty::Easy ? "Easy" : "Hard");
				menu.printOKInGreen(28, 10);
				messageOnScreen = true;
				messageTimestamp = GetTickCount();
			}
			else if (pressedKey == ESC) {//Case: returning to main menu
				menu.ResetMenu(); //Resetting menu variables
				firstPrint = true;
				clearScr(); //Clear the screen before returning to the menu
				menu.printMainMenu(); //Display menu again
				state = GameState::Standby;
				menu.resetAllArrows();
			}
			break;
		}
	}

	if (messageOnScreen && GetTickCount() - messageTimestamp >= 2000)
	{
		gotoxy(27, 10);
		std::cout << std::string(30, ' '); // Clear message
		messageOnScreen = false;
	}
}

// This function initiates the gameplay, manages the snake movement, collision checks, and apple interactions
void GameManager::playGame() {
	static int ticks = 0;

	while (state == GameState::Playing) {
		if (gameStart) {
			board.reset();
			resetEnemies();
			gameStart = false;
		}

		// Handle user input
		if (_kbhit())
		{
			char key = _getch();
			if (key == ESC)
			{
				state = GameState::Pause;
				return;
			}

			mario->setPressedKey(key);
		}

		// Enemies movement
		if (ticks % 2 == 0)
			updateBarrels();

		if (ticks % 4 == 0)
			updateGhosts();

		if (ticks % 100 == 0)
			donkeyKongThrowsNewBarrel();

		mario->move();
		pauline->updateWinCondition(mario->getMarioX(), mario->getMarioY());
		if (checkWinCondition())
			state = GameState::GameWon;
		else {
			auto report = checkIfMarioHit();
			if (report.source != DamageSource::None)
				handleMarioDeath();
		}
		
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

		// TODO: handle game logic if we know we hit mario!

		if (barrelIterator->isExploded())
			barrelIterator = barrels.erase(barrelIterator);
		else
			++barrelIterator;
	}
}

void GameManager::updateGhosts() {
	for (auto ghostIterator = ghosts.begin(); ghostIterator != ghosts.end();) {
		(*ghostIterator)->move();

		// TODO: handle game logic if we know we hit mario!

		if ((*ghostIterator)->amIDead())
			ghostIterator = ghosts.erase(ghostIterator);
		else
			++ghostIterator;
	}
}

void GameManager::resetEnemies() {
	barrels.clear();
	ghosts.clear();

	readGhosts(board.getLevel());
	initializeDonkeyKong(board.getLevel());
}

void GameManager::readGhosts(const Level& level) {
	const auto& lvl = level.getOriginalLevel();
	const auto& ghostSpawns = level.getGhostsSpawnPoints();
	unsigned int seedForSmallGhost = randomizeSeedForSmallGhost();

	for (const auto& spawn : ghostSpawns) {
		
		char icon = lvl[spawn.second][spawn.first];

		if (icon == SmallGhost::SMALL_GHOST_ICON)
			ghosts.push_back(std::make_unique<SmallGhost>(spawn.first, spawn.second, isColor, board, seedForSmallGhost));
		else if (icon == BigGhost::BIG_GHOST_ICON)
			ghosts.push_back(std::make_unique<BigGhost>(spawn.first, spawn.second, isColor, mario->getMarioXRef(), mario->getMarioYRef(), board));
	}
}

void GameManager::initializeDonkeyKong(const Level& level) {
	if (donkeyKong == nullptr)
		donkeyKong = new DonkeyKong(level.getDonkeyKongSpawnX(), level.getDonkeyKongSpawnY(), isColor, board);
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
	
	// TODO: add sound of mario losing life using the Beep method
	if (mario->marioLifePoints() <= 0) {
		state = GameState::GameOver;
		clearScr();
		/*
		 * Make all of this inside the gameOverLogic() function, and call it somewhere else
		 std::cout << gameOverScreen;
		// TODO: add sound of game over using the Beep method
		 */
	}
	else {
		mario->reset();
		board.reset();
		ghosts.clear();
		readGhosts(board.getLevel());
		barrels.clear();
	}
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