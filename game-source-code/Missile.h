// Missile.h
#pragma once
#include <SFML/Graphics.hpp>

/**
 * @class Missile
 * @brief Represents a missile fired by the player.
 *
 * This class defines the behavior and attributes of a player's missile.
 */

class Missile
{
public:
    /**
     * @brief Constructor for the Missile class.
     *
     * @param x The initial X-coordinate of the missile.
     * @param y The initial Y-coordinate of the missile.
     * @param targetPosition The target position that the missile is aiming for.
     */
    Missile(float x, float y, const sf::Vector2f &targetPosition);

    /**
     * @brief Update the missile's position and behavior over time.
     *
     * @param deltaTime The time elapsed since the last update.
     */
    void update(float deltaTime);

    /**
     * @brief Draw the missile on the specified render window.
     *
     * @param window The SFML render window where the missile will be drawn.
     */
    void draw(sf::RenderWindow &window);

    /**
     * @brief Get the bounding rectangle of the missile.
     *
     * @return The bounding rectangle of the missile.
     */
    sf::FloatRect getBounds() const;

     /**
     * @brief Check if the missile is out of bounds.
     *
     * @return True if the missile is out of bounds, false otherwise.
     */
    bool isOutOfBounds() const;

    /**
     * @brief this defines the speed of the missile
     */
    float speed;

private:
    sf::RectangleShape shape;
    
    sf::Vector2f targetPosition;
    sf::Vector2f direction;
    sf::Vector2f missileDirection;  
};
