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

    // Data members
    int ticks;
    unsigned int score;
    unsigned int logicalRefreshRateMs;
    unsigned int renderDelayMs;
    bool singleLevelMode;
    unsigned int currentLevelSeed;

    // Game settings
    bool& isColor;
    const Difficulty& difficultyLevel;

    // References to game objects
    Board& board;
    Mario* mario;
    Pauline* pauline;
    DonkeyKong* donkeyKong;
    Legend* legend;

	// Enemy management
    std::list<Barrel> barrels;
    std::vector<std::unique_ptr<Ghost>> ghosts;

    // Item management
    Hammer* uncollectedHammer;
    std::list<ExtraLife*> uncollectedExtraLives;

    // Level management
    ScreenLoader screenLoader;
    std::list<Level*> levels;
    std::list<Level*>::iterator currentLevel;
    std::unordered_map<std::string, std::string> error_log;

    // Load and save management
    InputProvider* inputProvider;
    ConsoleRenderer* renderer;
    GameObserver* observer;

    // Mario damage management
    enum class DamageSource { None, Barrel, Ghost, DonkeyKong, Fall };
    struct MarioDamageReport {
        const Enemy* sourceEntity = nullptr; // nullptr for Fall or None
        DamageSource source = DamageSource::None;
    };

    // Level setup
    void setupNewLevel();
    void prepareLevelData();

    // Score management
    void manageScore();
    void addScore(int points);
    void triggerLegendBump();

	// Entity initialization
    void initializeMario(const Level& level);
    void initializePauline(const Level& level);
    void initializeDonkeyKong(const Level& level);
    void initializeHammer(const Level& level);
    void readGhosts(const Level& level, unsigned int seed);
    void readExtraLives(const Level& level);

    // Per-tick updates
    void donkeyKongThrowsNewBarrel();
    void updateBarrels();
    void updateGhosts();
    void tryCollectExtraLives();

    // Mario hit detection
    MarioDamageReport checkIfMarioHit() const;
    void handleMarioDeath();

    // Win condition
    bool checkWinCondition() const;

    // Reset & cleanup
    void resetEnemies();
    void clearAllEntities();
    void clearAllLevels();

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
        currentLevelSeed = 0;
    }

	~GameManager(); // Destructor

    // Game flow & state management
    void loadAllScreens();
    void startNewGame();
    void startSpecificLevel(const std::string& filename);
    enum class GameResult { InProgress, Won, Lost, Paused, QuitToMenu };
    GameResult playGame();

	// Getters
    bool hasLevels() const { return !levels.empty(); }
    const std::list<Level*>& getLevels() const { return levels; }
    const std::unordered_map<std::string, std::string>& getErrorLog() const { return error_log; }
    
    // Setters
    void setColor(bool color) { isColor = color; }
    void setDelayTimer(unsigned int ms) { logicalRefreshRateMs = ms; renderDelayMs = ms; }
};