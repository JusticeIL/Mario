#include <iostream>
#include "HelperFunc.h"
#include "Board.h"
#include "Menu.h"
#include "KeyboardInput.h"
#include "ConsoleRenderer.h"
#include "FileInput.h"
#include "FileRecorder.h"
#include "FileVerifier.h"
#include "GameManager.h"

int main(int argc, char* argv[]) {
	ShowConsoleCursor(false);

	bool isLoad = false;
	bool isSave = false;
	bool isSilent = false;
	chooseGameMode(argc, argv, isLoad, isSave, isSilent);

	Board gameBoard;
	gameBoard.setSilent(isSilent);

	InputProvider* input = nullptr;
	ConsoleRenderer* renderer = nullptr;
	GameObserver* observer = nullptr;

	// Renderer setup
	if (!isSilent)
		renderer = new ConsoleRenderer();

	if (isLoad) {
		input = new FileInput();
		observer = new FileVerifier();

		bool isColor = false;
		GameManager gm(gameBoard, Difficulty::Easy, isColor, input, renderer, observer);
		gm.loadAllScreens();
		gm.startNewGame();

		try {
			gm.playGame();
			std::cout.clear();
			clearScr();

			if (isSilent) {
				std::string passMsg1 = "Test Passed:";
				std::string passMsg2 = "All results matched the expected output!";

				int startX1 = (Board::MAX_X - static_cast<int>(passMsg1.length())) / 2;
				int startX2 = (Board::MAX_X - static_cast<int>(passMsg2.length())) / 2;
				int startY = Board::MAX_Y / 2;

				gotoxy(startX1, startY - 1);
				std::cout << GREEN << passMsg1 << RESET << std::endl;
				gotoxy(startX2, startY);
				std::cout << GREEN << passMsg2 << RESET << std::endl;
			}
		}
		catch (const std::exception& e) {
			if (isSilent) {
				std::string errMsg = e.what();
				int startX = (Board::MAX_X - static_cast<int>(errMsg.length())) / 2;
				int startY = Board::MAX_Y / 2;
				
				if (startX < 0) // Case: error message is too long for the console width
					startX = 0;

				clearScr();
				gotoxy(startX, startY);
				std::cout << RED << errMsg << RESET << std::endl;
			}
		}
	}
	else { // Case: Regular or save
		input = new KeyboardInput();

		if (isSave)
			observer = new FileRecorder();

		Menu gameMenu(gameBoard, input, renderer, observer);
		gameMenu.run();
	}
	
	delete input;
	delete renderer;
	delete observer;

	gotoxy(0, Board::MAX_Y - 3);
	return 0;
}