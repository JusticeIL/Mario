#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Barrel.h"
#include "ExtraLife.h"
#include "GameObserver.h"
#include "Ghost.h"
#include "Hammer.h"
#include "InputProvider.h"
#include "Level.h"
#include "ScreenLoader.h"

// Forward declarations
class Board;
class DonkeyKong;
class Mario;
class Pauline;
class Legend;
class ConsoleRenderer;

enum class Difficulty { Easy, Hard };

class GameManager {
    // Constants
    static constexpr unsigned int MAX_INIT_SCORE = 10000;

    // Data Members
    bool& isColor;
    int ticks;
    unsigned int score;
    unsigned int logicalRefreshRateMs;
    unsigned int renderDelayMs;
    bool singleLevelMode;
    unsigned int currentLevelSeed;

    // References to game objects
    ScreenLoader screenLoader;
    Board& board;
    Mario* mario;
    Pauline* pauline;
    DonkeyKong* donkeyKong;
    Hammer* uncollectedHammer;
    std::list<ExtraLife*> uncollectedExtraLives;
    Legend* legend;
    std::list<Barrel> barrels;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    std::list<Level*> levels;
    std::list<Level*>::iterator currentLevel;

    // Load and save management
    InputProvider* inputProvider;
    ConsoleRenderer* renderer;
    GameObserver* observer;

    // Mario Damage Management
    enum class DamageSource { None, Barrel, Ghost, DonkeyKong, Fall };
    struct MarioDamageReport {
        const Enemy* sourceEntity = nullptr; // nullptr for Fall or None
        DamageSource source = DamageSource::None;
    };

    // Game State Management
    const Difficulty& difficultyLevel;
    std::unordered_map<std::string, std::string> error_log;

    // Level management
    void prepareLevelData();

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
    void readGhosts(const Level& level, unsigned int seed);
    void updateGhosts();

    // Hammer management
    void initializeHammer(const Level& level);

	// Extra life management
    void readExtraLives(const Level& level);
    void tryCollectExtraLives();

    // Memory management
    void clearAllEntities();

public:
    GameManager(Board& B, const Difficulty& difficulty, bool& isColor, InputProvider* input, ConsoleRenderer* renderer, GameObserver* observer)
	: board(B), difficultyLevel(difficulty), isColor(isColor), inputProvider(input), renderer(renderer), observer(observer) { // Constructor
        mario = nullptr;
        pauline = nullptr;
        donkeyKong = nullptr;
        uncollectedHammer = nullptr;
    	legend = nullptr;
        ticks = 0;
        score = MAX_INIT_SCORE;
        logicalRefreshRateMs = (difficultyLevel == Difficulty::Hard) ? 50 : 150;
        renderDelayMs = logicalRefreshRateMs;
        singleLevelMode = false;
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
    const std::list<Level*>& getLevels() const { return levels; }
    void startSpecificLevel(const std::string& filename);
    enum class GameResult { InProgress, Won, Lost, Paused, QuitToMenu };
    GameResult playGame();
    void loadAllScreens();

    // Error handling
    const std::unordered_map<std::string, std::string>& getErrorLog() const { return error_log; }
    
    // Setters
    void setColor(bool color) { isColor = color; }
    void setDelayTimer(unsigned int ms) { logicalRefreshRateMs = ms; renderDelayMs = ms; }
};