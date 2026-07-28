#pragma once
#include <string>
#include "GameManager.h"
class Menu {

	// Constants for menu layout and options
	static constexpr char arrow = '>';
	static constexpr char gameCh = '1';
	static constexpr char optionsCh = '2';
	static constexpr char instructionsCh = '3';
	static constexpr char exitCh = '9';

	// Struct to define menu option positions
	struct menuArrowPos { int x; int y; }; // Has to be positioned here to maintain the constexpr compiling

	// Positions of menu options
	static constexpr menuArrowPos PLAY = { 7,15 };
	static constexpr menuArrowPos OPTIONS = { 7,17 };
	static constexpr menuArrowPos INSTRUCTIONS = { 7,19 };
	static constexpr menuArrowPos EXIT = { 7,21 };
	static constexpr menuArrowPos positions[] = { PLAY, OPTIONS, INSTRUCTIONS, EXIT };

	// Menu-related variables
    bool chosen;
	char menuChar;
	bool isArrow;
	bool MenuChoice;

    // Menu layout
    std::string mainMenu[GameManager::MAX_Y][GameManager::MAX_X + 1] = { //+1 for \0 TODO later, change from const chat to string
        //01234567890123456789012345678901234567890123456789012345678901234567890123456789
         "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
         "Q                                                                             Q", // 1
         "Q         |=====\\     |====|   |=\\   |=| |=|  |=| |======| |=|   |=|          Q", // 2
         "Q         |=|   ==|  |=|  |=|  |==\\  |=| |=||===| |=|       |=| |=|           Q", // 3
         "Q         |=|   ==|  |=|  |=|  |=|=\\ |=| |===|    |======|   |===|            Q", // 4
         "Q         |=|   ==|  |=|  |=|  |=|==\\|=| |=||===| |=|         |=|             Q", // 5
         "Q         |======|    |====|   |=| |===| |=|  |=| |======|    |=|             Q", // 6
         "Q                                                                             Q", // 7
         "Q                   |=|  |=|   |====|  |=\\   |=| |=======|                    Q", // 8
         "Q                   |=||===|  |=|  |=| |==\\  |=| |=|                          Q", // 9
         "Q                   |===|     |=|  |=| |=|=\\ |=| |=| |===|                    Q", // 10
         "Q                   |=||===|  |=|  |=| |=|==\\|=| |=|   |=|                    Q", // 11
         "Q                   |=|  |=|   |====|  |=| |===| |=======|                    Q", // 12
         "Q                                                                             Q", // 13
         "Q                                                                             Q", // 14
         "Q         [1] Start a new Game                                                Q", // 15
         "Q                                                                             Q", // 16
         "Q         [2] Options                                                         Q", // 17
         "Q                                                                             Q", // 18
         "Q         [3] Instructions                                                    Q", // 19
         "Q                                                                             Q", // 20
         "Q         [9] Exit                                                            Q", // 21
         "Q                                      Use numbers and ENTER to navigate      Q", // 22
         "Q                                                                             Q", // 23
         "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 24
    };

    // Screens
    std::string instructionsScreen =
        "Q=============================================================================Q\n"
        "Q                                                                             Q\n"
        "Q                                 Instructions:                               Q\n"
        "Q                                 __                                          Q\n"
        "Q              Use 'a' or 'd to move left or right accordingly                Q\n"
        "Q                                                                             Q\n"
        "Q                     Use 'w' to jump or go up the ladder                     Q\n"
        "Q                                                                             Q\n"
        "Q                        Use 'x' to go down the ladder                        Q\n"
        "Q                                                                             Q\n"
        "Q                           Use 's' to stay in place           ...            Q\n"
        "Q                                                             (()))           Q\n"
        "Q                                                             {' ())          Q\n"
        "Q                    Goal: to reach the princess ('$') ->      ) (()          Q\n"
        "Q                                                           @ (   ()          Q\n"
        "Q                                                            \\7 \\ ) |         Q\n"
        "Q                                                            {)=== /\\         Q\n"
        "Q                                                             \\ |    |        Q\n"
        "Q                                                               |    |        Q\n"
        "Q                                                               |    |        Q\n"
        "Q                                                               |    |        Q\n"
        "Q                                                               |    |        Q\n"
        "Q         [9] Back to main menu                                 |    |        Q\n"
        "Q                                                               '~~~~'        Q\n"
        "Q                                                                /Y           Q\n"
        "Q=============================================================================Q";

    std::string optionsScreen =
        "Q=============================================================================Q\n"
        "Q                                                                             Q\n"
        "Q                                    Options:                                 Q\n"
        "Q                                    _______                                  Q\n"
        "Q                                                                             Q\n"
        "Q                          [5] Colors [   ]                                   Q\n"
        "Q                          [6] Console log                                    Q\n"
        "Q                          [7] difficulty level :                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q         [9] Back to main menu                                               Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q=============================================================================Q";

    std::string FileErrorLogScreen =
        "Q=============================================================================Q\n"
        "Q                                                                             Q\n"
        "Q   <q                             Console log:                          e>   Q\n"
        "Q                                    _______                                  Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q         [9] Back to main menu                                               Q\n"
        "Q                                                                             Q\n"
        "Q                                                                             Q\n"
        "Q=============================================================================Q";

public:
    Menu() { // Constructor
        chosen = false;
        menuChar = '\0';
        isArrow = false;
        MenuChoice = false;
    }

    // Menu Display
    void printMainMenu() const;
    void printInstructionsScreen() const { std::cout << instructionsScreen; }
    void printOptionsScreen() const { std::cout << optionsScreen; }
    char handleMenu();
    void printOKInGreen(int x, int y) const;

    // Menu Navigation
    void MoveArrow(char numKey) const;
    void resetAllArrows() const;
    void ChangeisArrowChoice() { isArrow = true; }
    void ChangeMenuChar(char ch) { menuChar = ch; }
    void ResetMenu() { menuChar = '\0'; chosen = false; }

    // Getters
    bool GetMenuChoice() const { return chosen; }
};