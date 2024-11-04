#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Player.h"
#include "Laser.h"
#include "Lander.h"
#include "Humanoid.h"
#include "HighScore.h"
// initialise constant global variables
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;
const float PLAYER_SPEED = 5.0f;
const float LASER_SPEED = 10.0f;
const float LASER_COOLDOWN = 0.5f;

/**
 * @class Game
 * @brief The Game class represents the main game engine.
 *
 * This class is responsible for managing the game, including player interactions, scoring, and game state.
 */
class Game
{
public:
    /**
     * @brief Construct a new Game object.
     */
    Game();

    /**
     * @brief Run the game loop.
     */
    void run();

    /**
     * @brief Draw the splash screen.
     */
    void drawSplashScreen();
    sf::RenderTexture minimapTexture;
    sf::Clock intersectionCollisionTimer;
    sf::Clock frameClock; // intialise clock for timer synchronisation
    sf::Clock humanoidSpawnClock;
    sf::Clock shieldCooldown;
    sf::Clock missileSpawnTimer;
    sf::RenderWindow window;

    /**
     * @brief Check if the splash screen is currently displayed.
     *
     * @return True if the splash screen is displayed, false otherwise.
     */
    bool isSplashScreenDisplayed() const; // for test purposes

    /**
     * @brief Get the size of the game window.
     *
     * @return The size of the game window as an SFML Vector2u.
     */
    sf::Vector2u getWindowSize() const;   // for test purposes

    /**
     * @brief Spawn a lander.
     */
    void spawnLander();

    /**
     * @brief Update the scoreboard.
     */
    void updateScoreboard();

    /**
     * @brief Spawn missiles from active landers.
     */
    void spawnMissilesFromLanders();
    Lander *activeLander;
    sf::Texture humanoidTexture;
    sf::Sprite backgroundSprite;
    sf::Vector2f backgroundPosition;

    /**
     * @brief Destructor for the Game class.
     */
    ~Game();
    bool gameOver;
    bool isGameActive;
    bool isGameOverScreenDisplayed;
    bool allHumanoidsDead;
    bool typingName; // this is needed to stop name character inputs from being interpreted as commands
    sf::Clock playerDotUpdateTimer;
    Player player;

    /**
     * @brief Show the game over screen.
     */
    void showGameOverScreen();

    /**
     * @brief Reset the game to its initial state.
     */
    void resetGame();
    sf::Texture minimapBackgroundTexture;
    sf::Sprite minimapBackgroundSprite;

    /**
     * @brief Spawn humanoids at the bottom of the screen.
     */
    void spawnHumanoids();  // Spawns humanoids at the bottom of the screen.
    
    /**
     * @brief Update humanoid movement and interactions.
     */
    void updateHumanoids(); // Update humanoid movement and interactions.
    
    /**
     * @brief Draw humanoids on the game window.
     */
    void drawHumanoids();

    /**
     * @brief Check collisions between landers and humanoids.
     */
    void checkLanderHumanoidCollisions(Lander &lander); // std::vector<Lander>::iterator it);

    /**
     * @brief Check collisions between the player and humanoids.
     */
    void checkPlayerHumanoidCollision();

    /**
     * @brief Get the number of landers destroyed.
     *
     * @return The number of landers destroyed.
     */
    int getNumLandersDestroyed();

    /**
     * @brief Set the game as won.
     */
    void setGameWon();

    /**
     * @brief Check if the game is won.
     *
     * @return True if the game is won, false otherwise.
     */
    bool getGamewon();

    /**
     * @brief Check if the game over screen is displayed.
     *
     * @return True if the game over screen is displayed, false otherwise.
     */
    bool getIsGameOverScreenDisplayed();


private:
    int score;
    int numLives;
    int numShields;
    int numHumanoids;
    HighScore highScoreManager; // Create an instance of the HighScore class
    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text shieldsText;
    sf::Text humanoidText;
    sf::Text fuelText;
    sf::RectangleShape shieldFrame;
    bool shieldOn;
    std::vector<Laser> lasers;
    sf::Texture backgroundTexture;
    sf::SoundBuffer explosionBuffer;
    sf::Sound explosionSound;
    sf::SoundBuffer intersectionBuffer;
    sf::Sound intersectionSound;
    sf::SoundBuffer HumanoidBuffer;
    sf::Sound HumanoidSound;
    sf::SoundBuffer missileBuffer;
    sf::Sound missileSound;
    sf::SoundBuffer shieldBuffer;
    sf::Sound shieldSound;
    sf::SoundBuffer crashBuffer;
    sf::Sound crashSound;
    Lander lander;
    bool splashScreenDisplayed; // for test purposes
    std::vector<Missile> missiles;
    std::vector<Lander> landers;

    std::vector<Humanoid> humanoids;

    /**
     * @brief Create a new lander.
     */
    void createLander();
    sf::Clock spawnTimer;
    sf::Clock newGameTimer;
    sf::Clock collisionTimer;
    int totalLandersSpawned;
    int numLandersDestroyed;
    int numHumanoidsInTotal;
    bool gameWon; // this is a flag to indicate if the player has won the game
    
    /**
     * @brief Check if the game is won.
     *
     * @return True if the game is won, false otherwise.
     */
    bool isGameWon() const;

    /**
     * @brief Show the win screen.
     */
    void showWinScreen();
    

    std::vector<sf::Vector2f> humanoidPositions;
};

#endif