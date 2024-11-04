#ifndef HUMANOID_H
#define HUMANOID_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>


/**
 * @class Humanoid
 * @brief Represents a humanoid character in the game.
 *
 * The Humanoid class defines the behavior and attributes of humanoid characters in the game.
 */

class Humanoid
{
public:
    /**
     * @brief Default constructor for the Humanoid class.
     */
    Humanoid();

    /**
     * @brief Constructor for the Humanoid class with an initial position and texture.
     *
     * @param startX The initial X-coordinate of the humanoid.
     * @param startY The initial Y-coordinate of the humanoid.
     * @param texture The texture used for the humanoid's sprite.
     */
    Humanoid(float startX, float startY, const sf::Texture &texture);
    
    /**
     * @brief Destructor for the Humanoid class.
     */
    ~Humanoid();

    /**
     * @brief Update the humanoid's movement and state.
     *
     * @param numOfHumanoids The number of remaining humanoids in the game.
     */
    void update(int& numOfHumanoids); // Update humanoid movement and state.
    
    /**
     * @brief Draw the humanoid on the screen.
     *
     * @param window The SFML render window where the humanoid will be drawn.
     */
    void draw(sf::RenderWindow &window); // Draw the humanoid on the screen.

    /**
     * @brief Check if the humanoid is captured.
     *
     * @return True if the humanoid is captured, false otherwise.
     */
    bool isCaptured() const; // Check if the humanoid is captured.
    
    /**
     * @brief Check if the humanoid is falling.
     *
     * @return True if the humanoid is falling, false otherwise.
     */
    bool isFalling() const;  // Check if the humanoid is falling.
    
    /**
     * @brief Check if the humanoid is destroyed.
     *
     * @return True if the humanoid is destroyed, false otherwise.
     */
    bool isDestroyed() const;

    /**
     * @brief Check if the player has captured the humanoid.
     *
     * @return True if the player has captured the humanoid, false otherwise.
     */
    bool isPlayerCaptured() const;

    /**
     * @brief Capture the humanoid at the specified position.
     *
     * @param capturePos The position at which the humanoid is captured.
     */
    void capture(sf::Vector2f capturePos);   // Capture the humanoid.
    
    /**
     * @brief Release the humanoid.
     */
    void release();   // Release the humanoid.
    
    /**
     * @brief Start the humanoid falling.
     */
    void startFalling(); // Start the humanoid falling.
    
    /**
     * @brief Deposit the humanoid at the specified X-coordinate.
     *
     * @param x The X-coordinate at which the humanoid is deposited.
     */
    void deposit(float x);
    
    /**
     * @brief Set the humanoid's destroy state.
     */
    void setDestroy();

    /**
     * @brief Set whether the player has captured the humanoid.
     *
     * @param value True if the player has captured the humanoid, false otherwise.
     */
    void setPlayerCaptured(bool value);

    /**
     * @brief Get the position of the humanoid.
     *
     * @return The position of the humanoid as an sf::Vector2f.
     */
    sf::Vector2f getPosition() const; // Get the position of the humanoid.

    /**
     * @brief Get the bounding rectangle of the humanoid.
     *
     * @return The bounding rectangle of the humanoid as an sf::FloatRect.
     */
    sf::FloatRect getBounds();

private:
    //sf::RectangleShape humanoidShape;
    sf::Sprite humanoidShape;   // Changed from RectangleShape to Sprite
    sf::Texture texture; // Texture for the sprite
    sf::Vector2f velocity;
    bool captured;
    bool falling;
    bool destroy;
    bool PlayerCaptured;
    sf::Vector2f capturedPosition;
};

#endif
