#pragma once

#include <SFML/Graphics.hpp>
#include "Laser.h"
#include "Missile.h"
#include "Humanoid.h"

/**
 * @class Lander
 * @brief Represents an alien lander in the game.
 *
 * The Lander class manages the behavior and attributes of alien landers in the game,
 * including movement, collisions, missile firing, and interaction with humanoids.
 */
class Lander
{
public:
    /**
     * @brief Default constructor for the Lander class.
     */
    Lander();

    /**
     * @brief Constructor for the Lander class with a spawn cooldown.
     *
     * @param spawnCooldown The time interval between lander spawns.
     */
    Lander(float spawnCooldown);

    /**
     * @brief Set the position of the lander.
     *
     * @param newPosition The new position of the lander.
     */
    void setPosition(const sf::Vector2f &newPosition);

    /**
     * @brief Update the state and behavior of the lander.
     *
     * @param deltaTime The time passed since the last update.
     * @param playerPosition The position of the player's ship.
     * @param humanoidPositions The positions of humanoids.
     */
    void update(float deltaTime, const sf::Vector2f &playerPosition, std::vector<sf::Vector2f> humanoidPositions);

    /**
     * @brief Get the current position of the lander.
     *
     * @return The current position of the lander as an sf::Vector2f.
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Draw the lander on the specified window.
     *
     * @param window The SFML render window where the lander will be drawn.
     */
    void draw(sf::RenderWindow &window);

    /**
     * @brief Check if the lander is destroyed.
     *
     * @return True if the lander is destroyed; otherwise, false.
     */
    bool isDestroyed() const;

    sf::Sprite landerSprite; /**< The SFML sprite representing the lander. */
    
    /**
     * @brief Get the bounding rectangle of the lander.
     *
     * @return The bounding rectangle of the lander as an sf::FloatRect.
     */
    sf::FloatRect getLanderBounds() const;

    /**
     * @brief Get the texture of the lander.
     *
     * @return The texture of the lander as an sf::Texture.
     */
    const sf::Texture &getTexture() const;

    /**
     * @brief Get the texture of the lander.
     *
     * @return The texture of the lander as an sf::Texture.
     */
    sf::Texture landerTexture; /**< The texture for the lander. */

    /**
     * @brief Check if the lander collides with a laser.
     *
     * @param laser The laser to check for collision.
     * @return True if the lander collides with the laser; otherwise, false.
     */
    bool checkCollision(const Laser &laser);

    sf::FloatRect landerBounds; /**< The bounding rectangle of the lander. */

    /**
     * @brief Set the lander as destroyed.
     */
    void setDestroyed();

    /**
     * @brief Spawn a new lander.
     */
    void spawnLander();

    sf::Clock missileCooldown; /**< A clock to manage missile firing cooldown. */

    /**
     * @brief Check if the lander can fire a missile.
     *
     * @return True if the lander can fire a missile; otherwise, false.
     */
    bool canFireMissile();

    /**
     * @brief Helper method to check if a missile can be fired.
     *
     * @return True if a missile can be fired; otherwise, false.
     */
    bool canFireMissileSimulated();

    /**
     * @brief Fire a missile towards the player's ship.
     *
     * @param playerPosition The position of the player's ship.
     */
    void fireMissile(const sf::Vector2f &playerPosition);

    /**
     * @brief Check if the lander has captured a humanoid.
     */
    void hasCapturedHumanoid();

    /**
     * @brief Check if a humanoid is captured.
     *
     * @return True if a humanoid is captured; otherwise, false.
     */
    bool humanoidIsCaptured();

    /**
     * @brief Check if a captured humanoid is destroyed.
     *
     * @return True if a captured humanoid is destroyed; otherwise, false.
     */
    bool checkHumanoidDestroyed();

private:
     bool destroyed; /**< Flag indicating if the lander is destroyed. */
    sf::Clock fireCooldown; /**< A clock to manage missile firing cooldown. */
    std::vector<Missile> missiles; /**< A collection of missiles fired by the lander. */
    float spawnCooldown; /**< The time interval between lander spawns. */
    sf::Clock spawnTimer; /**< A clock to manage lander spawn cooldown. */
    sf::Vector2f moveTarget; /**< The target position for lander movement. */
    bool captured; /**< Flag indicating if a humanoid is captured by the lander. */
    bool humanoidDestroyed; /**< Flag indicating if the captured humanoid is destroyed. */
};