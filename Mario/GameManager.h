#pragma once
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Barrel.h"
#include "Ghost.h"
#include "Hammer.h"
#include "Level.h"
#include "ScreenLoader.h"

// Forward declarations
class Board;
class DonkeyKong;
class Mario;
class Pauline;
class Legend;

enum class Difficulty { Easy, Hard };

class GameManager {
    // Constants
    static constexpr unsigned int MAX_INIT_SCORE = 10000;

    // Data Members
    bool firstPrint;
    bool gameStart;
    bool paused;
    bool& isColor;
    int ticks;
    unsigned int score;
    unsigned int refreshRateMs;

    // References to game objects
    ScreenLoader screenLoader;
    Board& board;
    Mario* mario;
    Pauline* pauline;
    DonkeyKong* donkeyKong;
    Hammer* uncollectedHammer;
    Legend* legend;
    std::list<Barrel> barrels;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    std::list<Level*> levels;
    std::list<Level*>::iterator currentLevel;

    // Mario Damage Management
    enum class DamageSource { None, Barrel, Ghost, DonkeyKong, Fall };
    struct MarioDamageReport {
        const Enemy* sourceEntity = nullptr; // nullptr for Fall or None
        DamageSource source = DamageSource::None;
    };

    // Game State Management
    const Difficulty& difficultyLevel;
    std::unordered_map<std::string, std::string> error_log;

    // Score Management
    void manageScore();
    void addScore(int points);
    void triggerLegendBump();
    
    // Enemies
    void resetEnemies();

    // Game state
    bool checkWinCondition() const;

	// Mario life and death handling
    void initializeMario(const Level& level);
    MarioDamageReport checkIfMarioHit() const;
    void handleMarioDeath();
 
	// Donkey kong management
    void initializeDonkeyKong(const Level& level);
    void donkeyKongThrowsNewBarrel();

	// Pauline management
    void initializePauline(const Level& level);

    // Barrels management
    void updateBarrels();

    // Ghosts management
    unsigned int randomizeSeedForSmallGhost();
    void readGhosts(const Level& level);
    void updateGhosts();

    // Hammer management
    void initializeHammer(const Level& level);

    // Memory management
    void clearAllEntities();

public:
    GameManager(Board& B, const Difficulty& difficulty, bool& isColor) : board(B), difficultyLevel(difficulty), isColor(isColor) { // Constructor
        mario = nullptr;
        pauline = nullptr;
        donkeyKong = nullptr;
        uncollectedHammer = nullptr;
    	legend = nullptr;
        firstPrint = false;
        gameStart = false;
        paused = false;
        ticks = 0;
        score = MAX_INIT_SCORE;
        refreshRateMs = (difficultyLevel == Difficulty::Hard) ? 50 : 150;
    }

	~GameManager(); // Destructor

    // Constants
    static constexpr int MAX_X = 80;
    static constexpr int MAX_Y = 25;
    static constexpr int MIN_X = 0;
    static constexpr int MIN_Y = 0;

    // Game Flow & State Management
    void setupNewLevel();
    void startNewGame();
    bool hasLevels() const { return !levels.empty(); }
    enum class GameResult { InProgress, Won, Lost, Paused, QuitToMenu };
    GameResult playGame();
    void loadAllScreens();

    // Error handling
    const std::unordered_map<std::string, std::string>& getErrorLog() const { return error_log; }
    
    // Setters
    void setColor(bool color) { isColor = color; }
    void setDelayTimer(unsigned int ms) { refreshRateMs = ms; }
};