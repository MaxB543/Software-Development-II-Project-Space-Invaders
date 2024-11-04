#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>
#include <iostream>
class Laser;

/**
 * @class Player
 * @brief The Player class represents the player's character in a game.
 *
 * This class provides a representation of the player character with various attributes and functionality.
 */
class Player
{
public:
    /**
     * @brief Construct a new Player object.
     */
    Player();
    /**
     * @brief Set the player's game state.
     *
     * @param playing True if the player is currently playing the game, false otherwise.
     */
    void setPlayerState(bool playing);
    /**
     * @brief Handle player input and control the player character.
     *
     * @param window The SFML render window.
     * @param lasers A vector of Laser objects.
     */
    void handleInput(sf::RenderWindow &window, std::vector<Laser> &lasers);
    /**
     * @brief Update the player's character and game state.
     *
     * @param lasers A vector of Laser objects.
     */
    void update(std::vector<Laser> &lasers);

    /**
     * @brief Draw the player's character on the game window.
     *
     * @param window The SFML render window.
     */
    void draw(sf::RenderWindow &window);

    /**
     * @brief Move the player character to the right.
     */
    void moveRight();

    /**
     * @brief Move the player character to the left.
     */
    void moveLeft();

    /**
     * @brief Check if the game is currently being played.
     *
     * @return True if the player is currently playing, false otherwise.
     */
    bool isGamePlaying() const;

    /**
     * @brief Check if a successful fuel collision occurred.
     */
    bool successfulFuelCollision;

    /**
     * @brief Get the position of the player character.
     *
     * @return The position of the player character as an SFML vector.
     */
    sf::Vector2f getPlayerPosition() const;

    /**
     * @brief Get the bounding box of the player character.
     *
     * @return The bounding box of the player character as an SFML FloatRect.
     */
    sf::FloatRect getPlayerBounds() const;

    /**
     * @brief Start the game.
     */
    void startGame();
    float laserCooldownTimer;
    sf::Sprite PlayerSprite;
    bool isFacingRight; // this is to keep track of direction the ship is facing to orient the lasers properly
    sf::Texture PlayerTexture;
    sf::RectangleShape fuelBarOutline;
    sf::RectangleShape fuelBar;
    sf::Texture fuelCanTexture;
    sf::Sprite fuelCanSprite;

    //sf::RectangleShape fuelCan;
    /**
     * @brief Get the remaining fuel.
     *
     * @return The remaining fuel as a double.
     */
    double getFuel();

    /**
     * @brief Set the remaining fuel value.
     *
     * @param fuelValue The new fuel value.
     */
    void setFuel(double fuelValue);

    /**
     * @brief Set the position of the fuel can.
     */
    void setFuelCanPosition();

    /**
     * @brief Spawn a fuel can on the game window.
     *
     * @param window The SFML render window.
     */
    void spwanFuel(sf::RenderWindow &window);

    /**
     * @brief Handle a fuel can collision.
     */
    void fuelCanCollision();

    /**
     * @brief Set whether a humanoid is captured.
     *
     * @param value True if a humanoid is captured, false otherwise.
     */
    void setHumanoidCaptured(bool value);

    /**
     * @brief Check if a humanoid is captured.
     *
     * @return True if a humanoid is captured, false otherwise.
     */
    bool isHumanoidCaptured() const;

private:
    bool isPlaying;

    sf::Clock lastShotTime; // Add this variable to track the last shot time
    sf::SoundBuffer laserBuffer;
    sf::Sound laserSound;
    sf::SoundBuffer fuelBuffer;
    sf::Sound fuelSound;
    sf::Clock laserClock; // this clock manages the laser direction
    sf::Clock fuelClock;

    double fuel;
    bool hasFuelPowerUp;
    bool humanoidCaptured;
    
};

#endif