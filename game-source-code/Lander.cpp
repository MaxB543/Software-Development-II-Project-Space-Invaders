#include "Lander.h"
#include "Game.h"
#include "Humanoid.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <vector>
#include <cmath>

const int MISSILE_SHOT_COOLDOWN = 5.0f;
const int LANDER_HEIGHT = 30.0f;
const int MOVEMENT_SPEED = 2.0f;
const int HUMANOID_HEIGHT = 30.0f;

Lander::Lander(float spawnCooldown) : landerSprite(), destroyed(false), 
spawnCooldown(spawnCooldown), captured(false), humanoidDestroyed(false)
{
    if (!landerTexture.loadFromFile("resources/landership.png"))
    {
        std::cerr << "Failed to load lander texture" << std::endl;
    }
    else
    {
        sf::Vector2u textureSize = landerTexture.getSize();
    }
    landerTexture.setSmooth(true);
    landerSprite.setScale(0.2f, 0.2f);
    landerSprite.setTexture(landerTexture);
    landerSprite.setOrigin(landerSprite.getLocalBounds().width / 2, landerSprite.getLocalBounds().height / 2);
    spawnLander();
    spawnTimer.restart();
    missileCooldown.restart();
}

void Lander::update(float deltaTime, const sf::Vector2f &playerPosition, std::vector<sf::Vector2f> humanoidPositions)
{

    if (!destroyed && canFireMissile())
    {
        fireMissile(playerPosition);
    }

    if (!destroyed)
    {
        // this checks the missile cooldown and fire if it's ready
        if (missileCooldown.getElapsedTime().asSeconds() > 2)
        {
            fireMissile(playerPosition);
            missileCooldown.restart();
        }

        if(!captured)
        {
            //calculate move target
            int minDistance =1000;
            int xDistance = 0;
            int yDistance = 0;

            for (int i = 0; i < humanoidPositions.size(); i++)
            {
                xDistance = abs(humanoidPositions[i].x - landerSprite.getPosition().x);
                yDistance = abs(humanoidPositions[i].y - landerSprite.getPosition().y);

                if(minDistance > (xDistance + yDistance))
                {
                    minDistance = xDistance + yDistance;
                    moveTarget = humanoidPositions[i];
                }
            }

            // this calculates the direction to move towards the moveTarget
            sf::Vector2f direction = moveTarget - landerSprite.getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            direction /= distance; // this normalises the direction vector
            sf::Vector2f newPosition = landerSprite.getPosition() + direction * 40.0f * deltaTime;
            if(newPosition.x > 0 && newPosition.x < WINDOW_WIDTH && newPosition.y > 0 && newPosition.y < WINDOW_HEIGHT && moveTarget.y == (WINDOW_HEIGHT-100))
            {
                landerSprite.setPosition(newPosition); // Update the position unconditionally
            }
            else
            {
                if(newPosition.y < WINDOW_HEIGHT-110)
                {
                    landerSprite.move(0, MOVEMENT_SPEED);
                }
                else if(newPosition.x < WINDOW_WIDTH-50)
                {
                    landerSprite.move(MOVEMENT_SPEED, 0);
                }
                else if(newPosition.x > 50)
                {
                    landerSprite.move(-MOVEMENT_SPEED, 0);
                }
            }
        }
        else
        {
            if(landerSprite.getPosition().y >= 50)
            {
                landerSprite.move(0, -MOVEMENT_SPEED);
            }
            else
            {
                captured = false;
                //humanoidDestroyed = true;
            }
        }
    }
    else
    {
        captured = false;
    }
}

sf::Vector2f Lander::getPosition() const
{
    return landerSprite.getPosition();
}

void Lander::draw(sf::RenderWindow &window)
{
    if (!destroyed)
    {

        window.draw(landerSprite);
    }
}

bool Lander::isDestroyed() const
{
    return destroyed;
}

void Lander::setDestroyed()
{
    destroyed = true;
    landerSprite.setPosition(-50, -50);
}

sf::FloatRect Lander::getLanderBounds() const
{
    return landerSprite.getGlobalBounds();
}

const sf::Texture &Lander::getTexture() const
{
    return landerTexture;
}

bool Lander::checkCollision(const Laser &laser)
{
    // this gets the bounding boxes of the Lander and laser
    sf::FloatRect LanderBounds = landerSprite.getGlobalBounds();
    sf::FloatRect laserBounds = laser.shape.getGlobalBounds();
    if (LanderBounds.intersects(laserBounds))
    {
        destroyed = true;

        return true; // this means a collision is detected
    }

    return false; // this means no collision
}

void Lander::spawnLander()
{
    float x, y;

    // this keeps generating random positions until a valid one is found
    do {
        x = static_cast<float>(rand() % WINDOW_WIDTH);  
        y = 50;//rand() % (WINDOW_HEIGHT - 100) + 50; 
    } while (std::abs(x - moveTarget.x) < 100 && std::abs(y - moveTarget.y) < 100);

    landerSprite.setPosition(x, y);
    fireCooldown.restart();

    //this initialises moveTarget to a random position within a limited distance
    float offsetX = static_cast<float>(rand() % 200 - 100); // Adjust the range as needed
    float offsetY = static_cast<float>(rand() % 200 - 100); // Adjust the range as needed
    moveTarget = sf::Vector2f(x + offsetX, y + offsetY);
}

bool Lander::canFireMissile()
{
    return missileCooldown.getElapsedTime().asSeconds() >= MISSILE_SHOT_COOLDOWN;
}

bool Lander::canFireMissileSimulated()
{
    return missileCooldown.getElapsedTime().asSeconds() >= MISSILE_SHOT_COOLDOWN*0.000001f;

}

void Lander::fireMissile(const sf::Vector2f &playerPosition)
{
    // this calculates the position to spawn the missile (from the side of the Lander)
    float x = landerSprite.getPosition().x;
    float y = landerSprite.getPosition().y;

    // this calculates the direction towards the player
    sf::Vector2f direction = playerPosition - landerSprite.getPosition();
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= distance;
    missileCooldown.restart();
}

void Lander::setPosition(const sf::Vector2f &newPosition)
{
    landerSprite.setPosition(newPosition);
}

void Lander::hasCapturedHumanoid()
{
    captured = true;
}

bool Lander::humanoidIsCaptured()
{
    return captured;
}

bool Lander::checkHumanoidDestroyed()
{
    return humanoidDestroyed;
}