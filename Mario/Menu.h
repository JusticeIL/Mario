#pragma once
#include <string>
#include "GameManager.h"
#include "HelperFunc.h"

class Menu {

	// Constants for menu layout and options
	static constexpr char arrow = '>';
	static constexpr char PLAY_CH = '1';
    static constexpr char SPECIFIC_LEVEL_CH = '2';
	static constexpr char OPTIONS_CH = '3';
	static constexpr char INSTRUCTIONS_CH = '4';
	static constexpr char EXIT_CH = '9';

	// Struct to define menu option positions
	struct ScreenPos { int x; int y; }; // Has to be positioned here to maintain the constexpr compiling

	// Positions of menu options
	static constexpr ScreenPos PLAY = { 7,14 };
    static constexpr ScreenPos SPECIFIC_LEVEL = { 7,16 };
	static constexpr ScreenPos OPTIONS = { 7,18 };
	static constexpr ScreenPos INSTRUCTIONS = { 7,20 };
	static constexpr ScreenPos EXIT = { 7,22 };
	static constexpr ScreenPos positions[] = { PLAY, SPECIFIC_LEVEL, OPTIONS, INSTRUCTIONS, EXIT };

    // Positions of menu prints
	static constexpr ScreenPos COLOR_MODE_POS = { 39, 5 };
	static constexpr ScreenPos DIFFICULTY_POS = { 49, 7 };
    static constexpr ScreenPos OK_POS = { 60, 21 };

	// Menu state
    bool chosen;
	char menuChar;
	bool isArrow;
    bool firstPrint;
    
	// Game settings
    bool isColor;
    Difficulty difficultyLevel;

    // Current level index
    int selectedLevelIndex;

    // Game state
    GameManager gameManager;
    enum class GameState { MainMenu, Playing, SelectLevel, Instructions, Options, ConsoleLog, Pause, GameOver, GameWon, Exit };
    GameState state = GameState::MainMenu;

    // Screens
    std::string mainMenu =
        //01234567890123456789012345678901234567890123456789012345678901234567890123456789
        "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n" // 0
        "Q                                                                              Q\n" // 1
        "Q          |=====\\     |====|   |=\\   |=| |=|  |=| |======| |=|   |=|          Q\n" // 2
        "Q          |=|   ==|  |=|  |=|  |==\\  |=| |=||===| |=|       |=| |=|           Q\n" // 3
        "Q          |=|   ==|  |=|  |=|  |=|=\\ |=| |===|    |======|   |===|            Q\n" // 4
        "Q          |=|   ==|  |=|  |=|  |=|==\\|=| |=||===| |=|         |=|             Q\n" // 5
        "Q          |======|    |====|   |=| |===| |=|  |=| |======|    |=|             Q\n" // 6
        "Q                                                                              Q\n" // 7
        "Q                    |=|  |=|   |====|  |=\\   |=| |=======|                    Q\n" // 8
        "Q                    |=||===|  |=|  |=| |==\\  |=| |=|                          Q\n" // 9
        "Q                    |===|     |=|  |=| |=|=\\ |=| |=| |===|                    Q\n" // 10
        "Q                    |=||===|  |=|  |=| |=|==\\|=| |=|   |=|                    Q\n" // 11
        "Q                    |=|  |=|   |====|  |=| |===| |=======|                    Q\n" // 12
        "Q                                                                              Q\n" // 13
        "Q         [1] Start a new Game                                                 Q\n" // 14
        "Q                                                                              Q\n" // 15
        "Q         [2] Specific Level                                                   Q\n" // 16
        "Q                                                                              Q\n" // 17
        "Q         [3] Options                                                          Q\n" // 18
        "Q                                                                              Q\n" // 19
        "Q         [4] Instructions                                                     Q\n" // 20
        "Q                                                                              Q\n" // 21
        "Q         [9] Exit                     Use numbers and ENTER to navigate       Q\n" // 22
        "Q                                                                              Q\n" // 23
        "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"; // 24

    std::string instructionsScreen =
        "Q==============================================================================Q\n"
        "Q                                                                              Q\n"
        "Q                                 Instructions:                                Q\n"
        "Q                                 ____________                                 Q\n"
        "Q                                                                              Q\n"
        "Q              Use ' ' or ' ' to move left or right accordingly                Q\n"
        "Q                                                                              Q\n"
        "Q                     Use ' ' to jump or go up the ladder                      Q\n"
        "Q                                                                              Q\n"
        "Q                        Use ' ' to go down the ladder         ...             Q\n"
        "Q                                                             (()))            Q\n"
        "Q                           Use ' ' to stay in place          {' ())           Q\n"
        "Q                                                              ) (()           Q\n"
        "Q                    Goal: to reach the princess (' ') ->   @ (   ()           Q\n"
        "Q                                                            \\7 \\ ) |          Q\n"
        "Q                                                            {)=== /\\          Q\n"
        "Q                                                             \\ |    |         Q\n"
        "Q                                                               |    |         Q\n"
        "Q                                                               |    |         Q\n"
        "Q                                                               |    |         Q\n"
        "Q                                                               |    |         Q\n"
        "Q                                                               |    |         Q\n"
        "Q         [9] Back to main menu                                 '~~~~'         Q\n"
        "Q                                                                /Y            Q\n"
        "Q==============================================================================Q";

    std::string optionsScreen =
        "Q==============================================================================Q\n"
        "Q                                                                              Q\n"
        "Q                                    Options:                                  Q\n"
        "Q                                    _______                                   Q\n"
        "Q                                                                              Q\n"
        "Q                          [5] Colors [   ]                                    Q\n"
        "Q                          [6] Console log                                     Q\n"
        "Q                          [7] difficulty level:                               Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q         [9] Back to main menu                                                Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q==============================================================================Q";

    std::string FileErrorLogScreen =
        "Q==============================================================================Q\n"
        "Q                                                                              Q\n"
        "Q   <q                             Console log:                           e>   Q\n"
        "Q                                    _______                                   Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q         [9] Back to main menu                                                Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q==============================================================================Q";

    std::string gameOverScreen =
        "Q==============================================================================Q\n"
        "Q                                                                              Q\n"
        "Q          GGGGG  AAAAA  M     M  EEEEE       OOO   V   V  EEEEE  RRRR         Q\n"
        "Q         G       A   A  MM   MM  E          O   O  V   V  E      R   R        Q\n"
        "Q         G  GG   AAAAA  M M M M  EEEE       O   O  V   V  EEEE   RRRR         Q\n"
        "Q         G   G   A   A  M  M  M  E          O   O   V V   E      R  R         Q\n"
        "Q          GGGG   A   A  M     M  EEEEE       OOO     V    EEEEE  R   R        Q\n"
        "Q                                                                              Q\n"
        "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                    Game Over!                                  \n"
        "                               Returning To main menu                            ";


    std::string pauseScreen =
        "Q==============================================================================Q\n"
        "Q                                                                              Q\n"
        "Q                   PPPPP   AAAAA  U   U  SSSSS  EEEEE  DDDD                   Q\n"
        "Q                   P   P  A     A U   U  S      E      D   D                  Q\n"
        "Q                   PPPPP  AAAAAAA U   U  SSSSS  EEEE   D   D                  Q\n"
        "Q                   P      A     A U   U      S  E      D   D                  Q\n"
        "Q                   P      A     A UUUUU  SSSSS  EEEEE  DDDD                   Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q                                 _________                                    Q\n"
        "Q                                 |       |                                    Q\n"
        "Q                                 |  ESC  |                                    Q\n"
        "Q                                 |       |                                    Q\n"
        "Q                                 |_______|                                    Q\n"
        "Q                                                                              Q\n"
        "Q                       Press ESC to return to the game                        Q\n"
        "Q                                                                              Q\n"
        "Q                                                                              Q\n"
        "Q==============================================================================Q";

    std::string gameWonScreen =
        "Q==============================================================================Q\n"
        "Q                                                                              Q\n"
        "Q          Y   Y   OOOOO  U   U             WWW     WWW   OOOOO   N   N        Q\n"
        "Q           Y Y    O   O  U   U              W       W    O   O   NN  N        Q\n"
        "Q            Y     O   O  U   U              W   W   W    O   O   N N N        Q\n"
        "Q            Y     O   O  U   U              W  W W  W    O   O   N  NN        Q\n"
        "Q            Y     OOOOO   UUU                W     W     OOOOO   N   N        Q\n"
        "Q                                                                              Q\n"
        "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                                                                \n"
        "                                Congratulations!                                \n"
        "                                                                                ";

    std::string selectLevelScreen =
        "Q==============================================================================Q\n"
        "Q                               Select a Level                                 Q\n"
        "Q==============================================================================Q";

    // Menu Display
    void printMainMenu() const { gotoxy(0, 0); std::cout << mainMenu; }
    void printSelectLevelScreen() const;
    void printInstructionsScreen() const;
    void printOptionsScreen() const { gotoxy(0, 0); std::cout << optionsScreen; }
    void printConsoleLogScreen(size_t currentErrorPage);
    void printPauseScreen() const { gotoxy(0, 0); std::cout << pauseScreen; }
	void printGameOverScreen() const { gotoxy(0, 0); std::cout << gameOverScreen; }
	void printGameWonScreen() const { gotoxy(0, 0); std::cout << gameWonScreen; }
    void printScreens();
    void printOKInGreen() const;

    // Menu Navigation
    void MoveArrow(char numKey) const;
    void resetAllArrows() const;
    void updateSelectLevelArrow(int oldIndex, int newIndex) const;

    // Input handling
    char handleMenu();
    void handleSelectLevelInput(char pressedKey);
    void handleConsoleLogInput(size_t& currentErrorPage);
    void handleState();

    // Menu state
    void ChangeIsArrowChoice() { isArrow = true; }
    void ResetMenu() { menuChar = '\0'; chosen = false; }
    void terminatePause() { clearScr(); firstPrint = true; }

    // Game Logic
    void gameOverLogic();
    void gameWonLogic();
    void gameReset();

    // Sound FX
    void playWinSound() const;
    void playLoseSound() const;
    void playExitSound() const;

    // Getter
    bool GetMenuChoice() const { return chosen; }

public:
    Menu(Board& board, InputProvider* input, ConsoleRenderer* renderer, GameObserver* observer)
	: isColor(false), difficultyLevel(Difficulty::Easy), gameManager(board, difficultyLevel, isColor, input, renderer, observer) { // Constructor
        chosen = false;
        menuChar = '\0';
        isArrow = false;
        firstPrint = true;
		selectedLevelIndex = 0;
    }

    // Constants
    static constexpr char ESC_CH = 27;

    // Public entry point
    void run();
};