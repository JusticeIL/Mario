#include "GameManager.h"

//This function manages the main game loop, handles transitions between different game states, and coordinates user inputs and game actions
void GameManager::run() {

	menu.print();
	char pressedKey = '\0';

	while (State != GameState::Exit) //Game loop runs as long as ESC key wasn't pressed
	{
		if (State == GameState::Standby)
		{
			choiceHolder = menu.handleMenu();
			switch (choiceHolder) {
			case play: //Start game
				State = GameState::Playing;
				choiceHolder = '\0';
				menu.ResetMenu();
				menu.resetAllArrows();
				if (music)
					playBackgroundMusic();
				firstPrint = true;
				break;
			case instructions: //Instructions
				State = GameState::Instructions;
				choiceHolder = '\0';
				menu.ResetMenu();
				menu.resetAllArrows();
				firstPrint = true;
				break;
			case options: //Options
				State = GameState::Options;
				choiceHolder = '\0';
				menu.ResetMenu();
				menu.resetAllArrows();
				firstPrint = true;
				break;
			case quit: //Exit
				clearScr();
				State = GameState::Exit;
				playExitSound();
				break;
			}
		}

		if (State == GameState::Playing)
			playGame();
		else if (State == GameState::Pause)
		{
			printPauseScreen();
			board.pause();
			if (_kbhit())
			{
				pressedKey = _getch();
				if (pressedKey == ESC) { //Unpause only after ESC
					terminatePause();
					State = GameState::Playing;
				}
			}
		}
		else if (State == GameState::Colors)
		{
			choiceHolder = '\0';
			menu.ResetMenu();
			menu.resetAllArrows();
			firstPrint = true;
			gotoxy(0, 0);
			std::cout << ColorsScreen;
			handleColors();
		}

		else if (State == GameState::GameOver)
			gameOverLogic();

		else if (State == GameState::GameWon)
			gameWonLogic();

		if (State != GameState::Playing && State != GameState::Standby) //Case: menu screens
		{
			printScreens();
			handleState();
		}

	}
}
