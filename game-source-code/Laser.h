#ifndef LASER_H
#define LASER_H
#include <SFML/Graphics.hpp>


/**
 * @class Laser
 * @brief The Laser class represents a laser beam in a game.
 *
 * This class provides a representation of a laser beam with various attributes.
 */
class Laser
{
public:
    
    /**
     * @brief Construct a new Laser object.
     *
     * @param position The initial position of the laser.
     * @param reverseDirection Flag indicating whether the laser should move in the reverse direction.
     */
    Laser(sf::Vector2f position, bool reverseDirection = false);

    /**
     * @brief Move the laser.
     */
    void move();
    /**
     * @brief Check if the laser is out of bounds.
     *
     * @param windowWidth The width of the game window.
     * @return True if the laser is out of bounds, otherwise false.
     */
    bool isOutOfBounds(int windowWidth) const;
    /**
     * @brief Draw the laser on a game window.
     *
     * @param window The SFML render window on which to draw the laser.
     */

    void draw(sf::RenderWindow &window);
    /**
     * @brief Get the position of the laser.
     *
     * @return The position of the laser as an SFML vector.
     */
    sf::Vector2f getPosition() const;
    /**
     * @brief Get the bounding box of the laser.
     *
     * @return The bounding box of the laser as an SFML FloatRect.
     */
    sf::RectangleShape shape;
    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    };
    bool destroyed;
    /**
     * @brief Set the laser as destroyed.
     */
    void setDestroyed();
    /**
     * @brief Check if the laser is destroyed.
     *
     * @return True if the laser is destroyed, otherwise false.
     */
    bool isDestroyed() const;
    /**
     * @brief Set the position of the laser.
     *
     * @param newPosition The new position for the laser as an SFML vector.
     */

    void setPosition(const sf::Vector2f& newPosition) {
        shape.setPosition(newPosition);
    }

private:
};

#endif