#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Menu.h"

// This function prints the menu
void Menu::printMainMenu() const {

    gotoxy(0, 0);

    for (int i = 0; i < std::size(mainMenu) - 1; i++) 
        std::cout << mainMenu[i] << '\n';
    
    std::cout << mainMenu[std::size(mainMenu) - 1];
}

// This function handles user input in the menu, updates the selected option, and manages the menu display
char Menu::handleMenu() {

    while (true) {
        if (_kbhit()) {
            char MenuInput = _getch();
            if ((GameManager::PLAY <= MenuInput && MenuInput <= GameManager::INSTRUCTIONS) || MenuInput == GameManager::QUIT) {//Available keys
                resetAllArrows(); // Case: remove any current showing arrows
                MoveArrow(MenuInput); // Case: Arrow is showing on screen
                isArrow = true; // Case: Arrow is showing on screen so flag should be true now
                menuChar = MenuInput; // Menu input is entered
            }

            if ((MenuInput == '\n' || MenuInput == '\r') && isArrow) {// Only after arrow is showing & player pressed ENTER
                chosen = true;

                if (menuChar == GameManager::ESC)
                    gotoxy(0, 26);

                clearScr();
                return menuChar;
            }
        }
        Sleep(10); // Prevent 100% CPU Usage
    }
}

// This function receives a numeric key (as a character), updates the menu arrow position, and prints it
void Menu::MoveArrow(char numKey) const {

    resetAllArrows(); // Reset all
    switch (numKey) {
    case GameManager::PLAY:
        gotoxy(PLAY.x, PLAY.y);
        break;
    case GameManager::OPTIONS:
        gotoxy(OPTIONS.x, OPTIONS.y);
        break;
    case GameManager::INSTRUCTIONS:
        gotoxy(INSTRUCTIONS.x, INSTRUCTIONS.y);
        break;
    case GameManager::QUIT:
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

// This function prints "OK!" in green at the specified screen coordinates with a blinking effect
void Menu::printOKInGreen(int x, int y) const {

    gotoxy(x, y);
    std::cout << GREEN << "OK!" << RESET;
    Sleep(150);
    gotoxy(x, y);
    std::cout << GREEN_BG << "OK!" << RESET_BG;
    Sleep(150);
    gotoxy(x, y);
    std::cout << GREEN << "OK!" << RESET;
    Sleep(150);
    gotoxy(x, y);
    std::cout << GREEN_BG << "OK!" << RESET_BG;
    Sleep(150);
    gotoxy(x, y);
    std::cout << GREEN << "OK!" << RESET;
}