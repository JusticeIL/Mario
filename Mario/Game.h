#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Ghost.h"

class Game {

    bool paused; // TODO: init in ctor later to false
    bool isColor; // TODO: init in ctor later to false
    std::list<Barrel> barrels;
    std::vector<std::unique_ptr<Ghost>> ghosts;

public:
	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;
    static constexpr int MIN_X = 0;
    static constexpr int MIN_Y = 0;

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

    // Barrels management
    void updateBarrels();
    void resetBarrels();

    // Ghosts management
    static void readGhostsFromBoard(Board& b, long int seed);
    static void updateGhosts();
    static void deleteAllGhosts();
};