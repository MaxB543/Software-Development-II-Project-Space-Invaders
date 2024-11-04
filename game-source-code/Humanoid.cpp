#include "Humanoid.h"
#include <iostream>

const int MOVEMENT_SPEED = 2.0f;
const int WINDOW_HEIGHT = 900;

Humanoid::Humanoid()
{
}

Humanoid::Humanoid(float startX, float startY, const sf::Texture &texture)
    : humanoidShape(texture),
      velocity(sf::Vector2f(1.0f, 0.0f)), // Initial velocity (move left)
      captured(false), falling(false), destroy(false), PlayerCaptured(false)
{
    humanoidShape.setPosition(startX, startY);
    humanoidShape.setTexture(texture);

    // Adjust the size and origin as needed
    humanoidShape.setScale(0.125f, 0.125f); // Adjust the scale as needed
    //humanoidShape.setFillColor(sf::Color::Blue);
}

Humanoid::~Humanoid()
{
}

void Humanoid::update(int &numOfHumanoids)
{
    if (captured)
    {
        humanoidShape.setPosition(capturedPosition);
        if(capturedPosition.y <= 60 && !destroy  && !PlayerCaptured)
        {
            destroy = true;
            numOfHumanoids--;
        }
    }
    else if (falling)
    {
        humanoidShape.move(0, MOVEMENT_SPEED);
        if(humanoidShape.getPosition().y == WINDOW_HEIGHT)
        {
            destroy = true;
            numOfHumanoids--;
        }
    }
    else
    {
        // Handle regular movement logic (left and right).
        // Implement logic for moving left and right within boundaries.
        sf::Vector2f newPosition = humanoidShape.getPosition() + velocity; //* deltaTime;
        humanoidShape.setPosition(newPosition);

        // Implement logic to change direction when hitting boundaries.
        // Example: if (newPosition.x < leftBoundary) velocity.x = 1.0f; // Move right
        // Check if the humanoid is out of bounds and reverse the movement direction
        if (humanoidShape.getPosition().x < 50 || humanoidShape.getPosition().x > 1600-50)
        {
            velocity.x *= -1.0f; // Reverse the direction
        }
    }
}

void Humanoid::draw(sf::RenderWindow &window)
{
    window.draw(humanoidShape);
}

bool Humanoid::isCaptured() const
{
    return captured;
}

bool Humanoid::isFalling() const
{
    return falling;
}

bool Humanoid::isDestroyed() const
{
    return destroy;
}

bool Humanoid::isPlayerCaptured() const
{
    return PlayerCaptured;
}

void Humanoid::capture(sf::Vector2f capturePos)
{
    captured = true;
    falling = false;
    capturedPosition = capturePos;
    // Implement logic for being captured, e.g., stop movement or attach to capturing entity.
}

void Humanoid::release()
{
    captured = false;
    falling = true;
    // Implement logic for being released, e.g., start falling.
}

void Humanoid::startFalling()
{
    falling = true;
    // Implement logic to start falling.
}

void Humanoid::deposit(float x)
{
    captured = false;
    falling = false;
    humanoidShape.setPosition(x+50, WINDOW_HEIGHT-100);
}

void Humanoid::setDestroy()
{
    destroy = true;
}

void Humanoid::setPlayerCaptured(bool value)
{
    PlayerCaptured = value;
}

sf::Vector2f Humanoid::getPosition() const
{
    return humanoidShape.getPosition();
}

sf::FloatRect Humanoid::getBounds()
{
    return humanoidShape.getGlobalBounds();
}