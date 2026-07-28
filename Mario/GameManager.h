#pragma once
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "DonkeyKong.h"
#include "Ghost.h"
#include "HelperFunc.h"
#include "Menu.h"
#include "Level.h"
#include "Mario.h"
#include "Pauline.h"
#include "ScreenLoader.h"

// Forward declarations
class Board;
class Barrel;

class GameManager {

    // Data Members
    char choiceHolder;
    bool firstPrint;
    bool gameStart;
    bool paused;
    bool isColor;
    int ticks;

    // References to game objects
    Menu& menu;
    ScreenLoader screenLoader;
    Board& board;
    Mario* mario;
    Pauline* pauline;
    DonkeyKong* donkeyKong;
    std::list<Barrel> barrels;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    std::list<Level> levels;

    // Game State Management
    enum class GameState { Standby, Playing, Pause, GameOver, Instructions, Options, Exit, GameWon };
    GameState state = GameState::Standby;
    enum class Difficulty { Easy, Hard };
    Difficulty difficultyLevel = Difficulty::Easy;
    std::unordered_map<std::string, std::string> error_log;

	// Game screens
    std::string gameOverScreen =
        "Q=============================================================================Q\n"
        "Q                                                                             Q\n"
        "Q          GGGGG  AAAAA  M     M  EEEEE    OOO   V   V  EEEEE  RRRR           Q\n"
        "Q         G       A   A  MM   MM  E       O   O  V   V  E      R   R          Q\n"
        "Q         G  GG   AAAAA  M M M M  EEEE    O   O  V   V  EEEE   RRRR           Q\n"
        "Q         G   G   A   A  M  M  M  E       O   O   V V   E      R  R           Q\n"
        "Q          GGGG   A   A  M     M  EEEEE    OOO     V    EEEEE  R   R          Q\n"
        "Q                                                                             Q\n"
        "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                   Game Over!                                  \n"
        "                              Returning To main menu                           ";


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
        "Q=============================================================================Q\n"
        "Q                                                                             Q\n"
        "Q          Y   Y   OOOOO  U   U       !!!      WWW     WWW   OOOOO   N   N    Q\n"
        "Q           Y Y    O   O  U   U       !!!       W       W    O   O   NN  N    Q\n"
        "Q            Y     O   O  U   U       !!!       W   W   W    O   O   N N N    Q\n"
        "Q            Y     O   O  U   U                 W  W W  W    O   O   N  NN    Q\n"
        "Q            Y     OOOOO   UUU        !!!        W     W     OOOOO   N   N    Q\n"
        "Q                                                                             Q\n"
        "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                                                               \n"
        "                                Congratulations!                               \n"
        "                              ON TO THE NEXT STAGE!                              ";

    unsigned int randomizeSeedForSmallGhost();
    void resetEnemies();
    bool checkWinCondition() { return pauline->checkWinCondition(); }
    void updateBarrels();
    void updateGhosts();
    void donkeyKongThrowsNewBarrel() { barrels.push_back(*donkeyKong->createBarrel()); }
    bool checkIfMarioHit();
    void resetGameAfterMarioDeath();
    void handleMarioDeath();

public:

    GameManager(Menu& M, Board& B) : menu(M), board(B) { // Constructor
        mario = nullptr;
		pauline = nullptr;
        donkeyKong = nullptr;
        choiceHolder = '\0';
        firstPrint = false;
        gameStart = false;
    	paused = false;
		isColor = false;
        ticks = 0;
    }

    // Constants
    static constexpr int MAX_X = 80;
    static constexpr int MAX_Y = 25;
    static constexpr int MIN_X = 0;
    static constexpr int MIN_Y = 0;
    static constexpr char PLAY = '1';
    static constexpr char OPTIONS = '2';
    static constexpr char INSTRUCTIONS = '3';
    static constexpr char ESC = 27;
    static constexpr char QUIT = 27;

    // Game Flow & State Management
    void run();
    void playGame();
    void printScreens();
    void handleState();
    void gameOverLogic();
    void gameWonLogic();
    void gameReset();
    void terminatePause() { clearScr(); firstPrint = true; }
    void printPauseScreen() const { std::cout << pauseScreen; }
    void loadAllScreens();

    // Barrels management
    void resetBarrels();

    // Ghosts management
    void readGhostsFromBoard(unsigned int seed);
};