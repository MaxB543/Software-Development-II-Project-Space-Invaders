// Missile.cpp
#include "Missile.h"
#include <cmath>

Missile::Missile(float x, float y, const sf::Vector2f &playerPosition)
    : speed(200.0f)
{
    shape.setSize(sf::Vector2f(10, 20));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(x, y);
    shape.setScale(2.0f, 0.5f);

    // Calculate the direction vector towards the player
    sf::Vector2f direction = playerPosition - shape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
    {
        direction /= length;
    }

    missileDirection = direction; // Store the direction vector for future updates
}


void Missile::update(float deltaTime)
{
    
    shape.move(missileDirection * speed * deltaTime);
}


void Missile::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

sf::FloatRect Missile::getBounds() const
{
    return shape.getGlobalBounds();
}

bool Missile::isOutOfBounds() const
{
    // Define your logic to check if the missile is out of bounds
    // For example, check if the missile's position is above the window's top edge
    return shape.getPosition().y < -0.0f;
}