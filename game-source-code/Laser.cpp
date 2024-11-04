#include "Laser.h"

const float LASER_SPEED = 15.0f;

Laser::Laser(sf::Vector2f position, bool reverseDirection)
    : shape(sf::Vector2f(40, -5))
{
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);

    if (reverseDirection)
    {
        shape.setSize(sf::Vector2f(40, 5)); 
        shape.move(0, shape.getSize().y); 
    }
}

void Laser::move()
{
   
    if (shape.getSize().y > 0) // this is for moving to the right
    {
        shape.move(LASER_SPEED, 0);
    }
    else // this is for moving to the left
    {
        shape.move(-LASER_SPEED, 0);
    }
}

bool Laser::isOutOfBounds(int windowWidth) const
{
    return shape.getPosition().x > windowWidth+ 200.0f;
}

void Laser::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::Vector2f Laser::getPosition() const
{
    return shape.getPosition();
}

void Laser::setDestroyed()
{
    destroyed = true;
}

bool Laser::isDestroyed() const
{
    return destroyed;
}