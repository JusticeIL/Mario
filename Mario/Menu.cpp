#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include "Menu.h"
#include "Colors.h"
#include "Mario.h"
#include "Pauline.h"

// This function manages the main game loop, handles transitions between different game states, and coordinates user inputs and game actions
void Menu::run() {

	gameManager.loadAllScreens();

	while (state != GameState::Exit) { // main menu loop runs as long as ESC key wasn't pressed
		if (state == GameState::MainMenu) {
			printMainMenu();
			menuChar = handleMenu();
			switch (menuChar) {
			case PLAY_CH: // Start game
				if (!gameManager.hasLevels()) {
					clearScr();
					gotoxy(10, 10);
					std::cout << "Error: No valid level files found to start the game.\n";
					gotoxy(10, 11);
					std::cout << "Please verify your .screen files are in the working directory.\n";
					gotoxy(10, 13);
					std::cout << "Press 'ESC' key to return to the main menu...";

					bool escChPressed = false;
					while (!escChPressed) {
						if (_kbhit() && _getch() == ESC_CH)
							escChPressed = true;

						Sleep(5);
					}

					ResetMenu();
					resetAllArrows();
					firstPrint = true;
					break;
				}

				gameReset();
				state = GameState::Playing;
				ResetMenu();
				resetAllArrows();
				firstPrint = true;
				break;
			case SPECIFIC_LEVEL_CH:
				if (!gameManager.hasLevels()) {
					clearScr();
					gotoxy(10, 10);
					std::cout << "Error: No valid level files found to start the game.\n";
					gotoxy(10, 11);
					std::cout << "Please verify your .screen files are in the working directory.\n";
					gotoxy(10, 13);
					std::cout << "Press 'ESC' key to return to the main menu...";

					bool escChPressed = false;
					while (!escChPressed) {
						if (_kbhit() && _getch() == ESC_CH)
							escChPressed = true;

						Sleep(5);
					}

					ResetMenu();
					resetAllArrows();
					firstPrint = true;
					break;
				}

				state = GameState::SelectLevel;
				menuChar = '\0';
				ResetMenu();
				resetAllArrows();
				firstPrint = true;
				break;
				
			case INSTRUCTIONS_CH: // Instructions
				state = GameState::Instructions;
				ResetMenu();
				resetAllArrows();
				firstPrint = true;
				break;
			case OPTIONS_CH: // Options
				state = GameState::Options;
				ResetMenu();
				resetAllArrows();
				firstPrint = true;
				break;
			case EXIT_CH: // Exit
				clearScr();
				state = GameState::Exit;
				break;
			}
		}
		else if (state == GameState::Playing) {
			GameManager::GameResult result = gameManager.playGame();
			if (result == GameManager::GameResult::Paused)
				state = GameState::Pause;
			else if (result == GameManager::GameResult::Lost)
				state = GameState::GameOver;
			else if (result == GameManager::GameResult::Won)
				state = GameState::GameWon;
		}
		else if (state == GameState::Pause) {
			printPauseScreen();
			if (_kbhit()) {
				char pressedKey = _getch();
				if (pressedKey == ESC_CH) { // Unpause only after ESC
					clearScr();
					firstPrint = true;
					state = GameState::Playing;
				}
			}
		}
		else if (state == GameState::GameOver)
			gameOverLogic();
		else if (state == GameState::GameWon)
			gameWonLogic();
		else if (state == GameState::SelectLevel || state == GameState::Instructions || state == GameState::Options) { // Case: menu screens
			printScreens();
			handleState();
		}
	}
}

void Menu::printScreens() {

	switch (state) {
	case GameState::SelectLevel:
	case GameState::Instructions:
	case GameState::Options:
		menuChar = '\0';
		break;
	}

	if (firstPrint)
		clearScr();
	
	gotoxy(0, 0);

	switch (state)	{
	case GameState::SelectLevel:
		printSelectLevelScreen();
		break;
	case GameState::Instructions:
		printInstructionsScreen();
		break;
	case GameState::Options:
		printOptionsScreen();
		// Color mode printing
		gotoxy(COLOR_MODE_POS.x, COLOR_MODE_POS.y);
		if (isColor)
			std::cout << "ON ";
		else
			std::cout << "OFF";

		// Difficulty printing
		gotoxy(DIFFICULTY_POS.x, DIFFICULTY_POS.y);
		if (difficultyLevel == Difficulty::Easy)
			std::cout << "Easy";
		else // Case: difficultylevel == HARD
			std::cout << "Hard";
		break;
	}

	firstPrint = false;
}

// This function processes user inputs and updates the game state accordingly during non-gameplay states
void Menu::handleState() {

	static DWORD messageTimestamp = 0;
	static bool messageOnScreen = false;

	while (state == GameState::SelectLevel || state == GameState::Instructions || state == GameState::Options) {
		if (_kbhit()) {
			char pressedKey = _getch();
			pressedKey = tolower(pressedKey);
			menuChar = pressedKey;

			switch (state) {
			case GameState::SelectLevel:
				handleSelectLevelInput(pressedKey);
				break;
			case GameState::Instructions:
				if (pressedKey == EXIT_CH) { // Case: returning to main menu
					ResetMenu(); // Resetting menu variables
					firstPrint = true;
					clearScr(); // Clear the screen before returning to the menu
					state = GameState::MainMenu;
					resetAllArrows();
				}
				break;
			case GameState::Options:
				if (pressedKey == '5') {
					isColor = !isColor; // Toggle color mode
					gameManager.setColor(isColor);
					gotoxy(COLOR_MODE_POS.x, COLOR_MODE_POS.y);
					std::cout << (isColor ? "ON " : "OFF");
					printOKInGreen();
					messageOnScreen = true;
					messageTimestamp = GetTickCount();
				}
				else if (pressedKey == '6') {
					state = GameState::ConsoleLog;
					size_t currentPage = 0;
					clearScr();
					gotoxy(0, 0);
					std::cout << FileErrorLogScreen;

					while (state == GameState::ConsoleLog) {
						printConsoleLogScreen(currentPage);
						handleConsoleLogInput(currentPage);
					}

					firstPrint = true;
					printScreens();
				}
				else if (pressedKey == '7') {
					difficultyLevel = (difficultyLevel == Difficulty::Easy) ? Difficulty::Hard : Difficulty::Easy; // Toggle difficulty level
					gotoxy(DIFFICULTY_POS.x, DIFFICULTY_POS.y);
					std::cout << std::string(10, ' '); // Clear message
					gotoxy(DIFFICULTY_POS.x, DIFFICULTY_POS.y);
					std::cout << (difficultyLevel == Difficulty::Easy ? "Easy" : "Hard");
					gameManager.setDelayTimer((difficultyLevel == Difficulty::Hard) ? 50 : 150);
					printOKInGreen();
					messageOnScreen = true;
					messageTimestamp = GetTickCount();
				}
				else if (pressedKey == EXIT_CH) {// Case: returning to main menu
					ResetMenu(); // Resetting menu variables
					firstPrint = true;
					clearScr(); // Clear the screen before returning to the menu
					state = GameState::MainMenu;
					resetAllArrows();
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
}

// This function handles user input in the menu, updates the selected option, and manages the menu display
char Menu::handleMenu() {

    while (true) {
        if (_kbhit()) {
            char MenuInput = _getch();
            if ((PLAY_CH <= MenuInput && MenuInput <= INSTRUCTIONS_CH) || MenuInput == EXIT_CH) {// Available keys
                resetAllArrows(); // Case: remove any current showing arrows
                MoveArrow(MenuInput); // Case: Arrow is showing on screen
                isArrow = true; // Case: Arrow is showing on screen so flag should be true now
                menuChar = MenuInput; // Menu input is entered
            }

            if ((MenuInput == '\n' || MenuInput == '\r') && isArrow) { // Only after arrow is showing & player pressed ENTER
                chosen = true;

                if (menuChar == ESC_CH)
                    gotoxy(0, 26);

                clearScr();
                return menuChar;
            }
        }
        Sleep(10); // Prevent 100% CPU Usage
    }
}

void Menu::printInstructionsScreen() const {
	gotoxy(0, 0);
	std::cout << instructionsScreen;

	gotoxy(20, 5);
	std::cout << static_cast<char>(Key::Left);
	gotoxy(27, 5);
	std::cout << static_cast<char>(Key::Right);
	gotoxy(27, 7);
	std::cout << static_cast<char>(Key::Up);
	gotoxy(30, 9);
	std::cout << static_cast<char>(Key::Down);
	gotoxy(33, 11);
	std::cout << static_cast<char>(Key::Stay);
	gotoxy(51, 13);
	std::cout << Pauline::PAULINE_ICON;
}

void Menu::printConsoleLogScreen(size_t currentErrorPage) {
	// 1. Clean the inner screen (using ' ' to wipe previous messages)
	std::string blankLine(76, ' ');
	for (int i = 5; i <= 20; ++i) {
		gotoxy(2, i);
		std::cout << blankLine;
	}

	const auto& errorLog = gameManager.getErrorLog();

	// 2. Handle perfect loads (no errors)
	if (errorLog.empty()) {
		const std::string successMsg = "No errors found! All screens loaded successfully.";
		int startX = (GameManager::MAX_X - static_cast<int>(successMsg.length())) / 2;
		gotoxy(startX, 10);
		std::cout << GREEN << successMsg << RESET;
	}
	else {
		// 3. Convert unordered_map to a vector to index pages
		std::vector<std::pair<std::string, std::string>> pages(errorLog.begin(), errorLog.end());

		size_t safePage = currentErrorPage;

		if (safePage >= pages.size())
			safePage = pages.size() - 1;

		const auto& currentPageData = pages[safePage];

		// Print the title at line 5, exactly in the middle.
		// Assuming the title length is 15 exactly (e.g. "dkong_01.screen")
		int titleStartX = (GameManager::MAX_X - 15) / 2;
		gotoxy(titleStartX, 5);
		std::cout << currentPageData.first;

		// 4. Format the text for the CURRENT page dynamically
		std::vector<std::string> displayLines;
		const std::string& errors = currentPageData.second;
		size_t startPos = 0;
		size_t newlinePos = errors.find('\n');

		// Slice up to the newline characters
		while (newlinePos != std::string::npos) {
			displayLines.push_back(errors.substr(startPos, newlinePos - startPos));
			startPos = newlinePos + 1;
			newlinePos = errors.find('\n', startPos);
		}
		// Catch the final line
		if (startPos < errors.length())
			displayLines.push_back(errors.substr(startPos));

		// 5. Find the absolute longest line to calculate dead-center alignment
		size_t maxLength = 0;
		for (const std::string& line : displayLines) {
			if (line.length() > maxLength)
				maxLength = line.length();
		}

		int startX = (GameManager::MAX_X - static_cast<int>(maxLength)) / 2;
		if (startX < 2) // Case: error string is too long
			startX = 2;

		// 6. Print the aligned block starting from line 7
		int currentY = 7;
		std::cout << RED;
		for (const std::string& line : displayLines) {
			if (currentY >= 20) {
				gotoxy(startX, currentY);
				std::cout << RESET;
				std::cout << "... (More errors hidden)";
				break;
			}
			gotoxy(startX, currentY);
			std::cout << line;
			currentY++;
		}
		std::cout << RESET;
	}
}

void Menu::handleConsoleLogInput(size_t& currentErrorPage) {
	const auto& errorLog = gameManager.getErrorLog();
	char userChoice = '\0';

	while (userChoice != 'q' && userChoice != 'e' && userChoice != EXIT_CH && userChoice != ESC_CH) {
		if (_kbhit()) {
			userChoice = tolower(_getch());

			if (userChoice == 'e') {
				// Next Page
				if (!errorLog.empty() && currentErrorPage + 1 < errorLog.size()) 
					++currentErrorPage;
				else 
					userChoice = '\0';
			}
			else if (userChoice == 'q') {
				// Previous Page
				if (!errorLog.empty() && currentErrorPage > 0) 
					--currentErrorPage;
				else 
					userChoice = '\0';
			}
			else if (userChoice == EXIT_CH || userChoice == ESC_CH) 
				state = GameState::Options;
			else // Case: Unfamiliar key pressed
				userChoice = '\0';
		}

		Sleep(10);
	}
}

// This function receives a numeric key (as a character), updates the menu arrow position, and prints it
void Menu::MoveArrow(char numKey) const {

    resetAllArrows(); // Reset all
    switch (numKey) {
    case PLAY_CH:
        gotoxy(PLAY.x, PLAY.y);
        break;
	case SPECIFIC_LEVEL_CH:
		gotoxy(SPECIFIC_LEVEL.x, SPECIFIC_LEVEL.y);
		break;
    case OPTIONS_CH:
        gotoxy(OPTIONS.x, OPTIONS.y);
        break;
    case INSTRUCTIONS_CH:
        gotoxy(INSTRUCTIONS.x, INSTRUCTIONS.y);
        break;
    case EXIT_CH:
        gotoxy(EXIT.x, EXIT.y);
        break;
    }

    std::cout << arrow;
}

// This function erases all arrows in the menu
void Menu::resetAllArrows() const {

    for (int i = 0; i < std::size(positions); i++) {
        gotoxy(positions[i].x, positions[i].y);
        std::cout << ' ';
    }
}

void Menu::handleSelectLevelInput(char pressedKey) {
	int totalLevels = static_cast<int>(gameManager.getLevels().size());
	char toLowerPressedKey = static_cast<char>(tolower(pressedKey));
	int oldIndex = selectedLevelIndex;
	bool arrowMoved = false;

	if (toLowerPressedKey == static_cast<char>(Key::Up)) { // Case: Mario Up
		--selectedLevelIndex;
		if (selectedLevelIndex < 0) // Case: Go to end
			selectedLevelIndex = totalLevels - 1;

		arrowMoved = true;
	}
	else if (toLowerPressedKey == static_cast<char>(Key::Down)) { // Case: Mario Down
		++selectedLevelIndex;
		if (selectedLevelIndex >= totalLevels) // Case: Go to start
			selectedLevelIndex = 0;

		arrowMoved = true;
	}
	else if (toLowerPressedKey >= '1' && toLowerPressedKey <= '0' + totalLevels) { // Case: Direct number choice
		selectedLevelIndex = toLowerPressedKey - '1'; // Cast char to index
		arrowMoved = true;
	}
	else if (toLowerPressedKey == '\r' || toLowerPressedKey == '\n') { // Case: Enter to confirm choice
		auto levelIterator = gameManager.getLevels().begin();
		std::advance(levelIterator, selectedLevelIndex);

		gameManager.startSpecificLevel((*levelIterator)->getFilename());

		state = GameState::Playing;
		menuChar = '\0';
		ResetMenu();
		resetAllArrows();
		firstPrint = true;
		clearScr();
		return;
	}
	else if (toLowerPressedKey == ESC_CH) { // Case: ESC to go back to main menu
		ResetMenu();
		firstPrint = true;
		clearScr();
		state = GameState::MainMenu;
		resetAllArrows();
		return;
	}

	if (arrowMoved && oldIndex != selectedLevelIndex) // Case: Reprint the screen only if the arrow position changed
		updateSelectLevelArrow(oldIndex, selectedLevelIndex);
}

void Menu::updateSelectLevelArrow(int oldIndex, int newIndex) const {
	gotoxy(5, 5 + (oldIndex * 2)); // 1. Erase the old arrow
	std::cout << ' ';

	gotoxy(5, 5 + (newIndex * 2)); // 2. Draw the new arrow
	std::cout << arrow;
}

// This function prints "OK!" in green with a blinking effect
void Menu::printOKInGreen() const {
    gotoxy(OK_POS.x, OK_POS.y);
    std::cout << GREEN << "OK!" << RESET;
    Sleep(150);
    gotoxy(OK_POS.x, OK_POS.y);
    std::cout << GREEN_BG << "OK!" << RESET_BG;
    Sleep(150);
    gotoxy(OK_POS.x, OK_POS.y);
    std::cout << GREEN << "OK!" << RESET;
    Sleep(150);
    gotoxy(OK_POS.x, OK_POS.y);
    std::cout << GREEN_BG << "OK!" << RESET_BG;
    Sleep(150);
    gotoxy(OK_POS.x, OK_POS.y);
    std::cout << GREEN << "OK!" << RESET;
	Sleep(150);
	gotoxy(OK_POS.x, OK_POS.y);
	std::cout << GREEN_BG << "OK!" << RESET_BG;
	Sleep(150);
	gotoxy(OK_POS.x, OK_POS.y);
	std::cout << GREEN << "OK!" << RESET;
	gotoxy(OK_POS.x, OK_POS.y);
	std::cout << "   ";
}

void Menu::gameOverLogic() {
	clearScr();
	printGameOverScreen();
	// playWinSound();
	Sleep(1000);
	state = GameState::MainMenu;
	firstPrint = true;
	ResetMenu();
	resetAllArrows();
	clearScr();
}

void Menu::gameWonLogic() {
	clearScr();
	printGameWonScreen();
	// playWinSound();
	Sleep(1000);
	state = GameState::MainMenu;
	firstPrint = true;
	ResetMenu();
	resetAllArrows();
	clearScr();
}

void Menu::gameReset() {
	// Tell the GameManager to prepare a fresh board, enemies, and Mario states
	gameManager.startNewGame();

	menuChar = '\0';
	firstPrint = true;
	resetAllArrows();
}

// This function plays the melody of "Twinkle Twinkle Little Star" (while game over) using the Beep function
void Menu::playWinSound() const {
	// Twinkle Twinkle Little Star
	Beep(523, 500);  // C5 for 500ms
	Beep(523, 500);  // C5 for 500ms
	Beep(784, 500);  // G5 for 500ms
	Beep(784, 500);  // G5 for 500ms
	Beep(880, 500);  // A5 for 500ms
	Beep(880, 500);  // A5 for 500ms
	Beep(784, 700); // G5 for 1000ms

	Sleep(300);      // Pause

	Beep(659, 500);  // F5 for 500ms
	Beep(659, 500);  // F5 for 500ms
	Beep(587, 500);  // D5 for 500ms
	Beep(587, 500);  // D5 for 500ms
	Beep(523, 500);  // C5 for 500ms
	Beep(523, 500);  // C5 for 500ms
	Beep(587, 500);  // D5 for 500ms
}

// This function plays a short sound effect for exiting the game using the Beep function
void Menu::playExitSound() const {
	Beep(1500, 100);
	Sleep(50);
	Beep(1000, 50);
	Beep(1200, 50);
}

void Menu::printSelectLevelScreen() const {
	gotoxy(0, 0);
	std::cout << selectLevelScreen;

	int y = 5;
	int optionNumber = 1;
	int currentIndex = 0;

	for (const auto& lvl : gameManager.getLevels()) {
		gotoxy(5, y);

		std::cout << (currentIndex == selectedLevelIndex ? arrow : ' ') << " [" << optionNumber << "] " << lvl->getFilename();

		y += 2;
		++optionNumber;
		++currentIndex;
	}

	gotoxy(5, y + 2);
	std::cout << "[ESC] Back to Main Menu";
}