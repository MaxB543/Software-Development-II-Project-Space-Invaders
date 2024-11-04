#include "player.h"
#include "laser.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

// Constant global variables defined here
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 900;
const float PLAYER_SPEED = 5.0f;
const float LASER_SPEED = 10.0f;
const float LASER_COOLDOWN = 0.25f; // Reduced cooldown time
const float PLAYER_X_SIZE = 0.2f;
const float PLAYER_Y_SIZE = 0.2f;

// The following code generates the player and their various physical properties

Player::Player()
    : PlayerSprite(), isPlaying(false), isFacingRight(true), fuel(200), hasFuelPowerUp(false), humanoidCaptured(false)
{
    lastShotTime.restart(); // This restarts the clock

    if (!PlayerTexture.loadFromFile("resources/8bitship.png"))
    {
        std::cerr << "Failed to load 8bitship.png" << std::endl;
    }

    if (!laserBuffer.loadFromFile("resources/Gun.wav")) // the following are resources
    {
        std::cerr << "Failed to load gun.wav" << std::endl;
    }
    laserSound.setBuffer(laserBuffer);

if (!fuelCanTexture.loadFromFile("resources/fuelcan.png"))
{
     std::cerr << "Failed to load fuelcan.png" << std::endl;
}

if (!fuelBuffer.loadFromFile("resources/fuelsound.mp3")) // the following are resources
    {
        std::cerr << "Failed to load fuelsound.mp3" << std::endl;
    }
    fuelSound.setBuffer(fuelBuffer);

    PlayerSprite.setTexture(PlayerTexture);
    PlayerSprite.setPosition(WINDOW_WIDTH / 2 - PlayerSprite.getLocalBounds().width / 2,
                             WINDOW_HEIGHT / 2 - PlayerSprite.getLocalBounds().height / 2);

    PlayerSprite.setScale(PLAYER_X_SIZE, PLAYER_Y_SIZE); // Adjust the scale as needed

    PlayerTexture.setSmooth(true);

    fuelBar.setSize(sf::Vector2f(fuel/2, 10));
    fuelBar.setFillColor(sf::Color::Red);            // Set the initial fuel bar color
    fuelBar.setPosition(WINDOW_WIDTH - fuelBar.getSize().x - 20, 13); // Position at the top right corner

    fuelBarOutline.setSize(sf::Vector2f(220, 15));
    fuelBarOutline.setFillColor(sf::Color::Blue);            // Set the initial fuel bar color
    fuelBarOutline.setPosition(WINDOW_WIDTH - 130, 10); // Position at the top right corner

    fuelCanSprite.setTexture(fuelCanTexture);
    fuelCanSprite.setScale( 0.10f, 0.10f);
    // fuelCan.setFillColor(sf::Color::Yellow);
    setFuelCanPosition();

}

void Player::moveLeft()
{
    // this flips the sprite to face left
    PlayerSprite.setScale(-PLAYER_X_SIZE, PLAYER_Y_SIZE);
    isFacingRight = false;
}

void Player::moveRight()
{
    // this resets the sprite's scale to face right
    PlayerSprite.setScale(PLAYER_X_SIZE, PLAYER_Y_SIZE);
    isFacingRight = true;
}

// This code checks how the game reacts to inputs

void Player::handleInput(sf::RenderWindow &window, std::vector<Laser> &lasers)
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space && !isPlaying)
            {
                if (lastShotTime.getElapsedTime().asSeconds() >= LASER_COOLDOWN)
                {
                    isPlaying = true;
                    lastShotTime.restart();
                }
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }
    }

    // This controls the movement of the player across the screen, preventing it from exceeding the dimensions of the screen
    if (isPlaying)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && PlayerSprite.getPosition().y > 0)
        {
            PlayerSprite.move(0, -PLAYER_SPEED);
            fuel = fuel - 0.1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && PlayerSprite.getPosition().y + PlayerSprite.getGlobalBounds().height < WINDOW_HEIGHT)
        {
            PlayerSprite.move(0, PLAYER_SPEED);
            fuel = fuel - 0.1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && PlayerSprite.getPosition().x > 0.1 * WINDOW_WIDTH)
        {
            moveLeft();
            PlayerSprite.move(-PLAYER_SPEED, 0);
            fuel = fuel - 0.1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && PlayerSprite.getPosition().x + PlayerSprite.getGlobalBounds().width < WINDOW_WIDTH + 100)
        {
            moveRight();
            PlayerSprite.move(PLAYER_SPEED, 0);
            fuel = fuel - 0.1;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (lastShotTime.getElapsedTime().asSeconds() >= LASER_COOLDOWN)
            {
                auto laserX = PlayerSprite.getPosition().x + 30.0f; // this uses addition for left-facing player
                auto laserY = PlayerSprite.getPosition().y + PlayerSprite.getGlobalBounds().height / 2;

                // this adjusts the laser's starting position based on the player's direction
                if (!isFacingRight) // this checks if the player is facing left
                {
                    laserX -= PlayerSprite.getGlobalBounds().width; // this subtracts for left-facing player
                }

                auto LaserShotNew = Laser(sf::Vector2f(laserX, laserY), isFacingRight); // this pases the direction to the Laser constructor
                lasers.push_back(LaserShotNew);
                lastShotTime.restart(); // Reset the cooldown timer
                laserSound.play();
            }
        }
    }
}

void Player::update(std::vector<Laser> &lasers)
{
    for (auto &laser : lasers)
    {
        laser.move();
    }
    lasers.erase(std::remove_if(lasers.begin(), lasers.end(),
                                [](const Laser &laser)
                                {
                                    return laser.isOutOfBounds(WINDOW_WIDTH + 300);
                                }),
                 lasers.end());

    fuelBar.setSize(sf::Vector2f(fuel, 10));
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(PlayerSprite);
    window.draw(fuelBarOutline);
    window.draw(fuelBar);
    
}

bool Player::isGamePlaying() const
{
    return isPlaying;
}
sf::Vector2f Player::getPlayerPosition() const
{
    return PlayerSprite.getPosition();
}

void Player::startGame()
{
    isPlaying = true;
}

double Player::getFuel()
{
    return fuel;
}

void Player::setFuel(double fuelValue)
{
    fuel = fuelValue;
}

void Player::setFuelCanPosition()
{
    float x = static_cast<float>(rand() % WINDOW_WIDTH);
    float y = static_cast<float>(WINDOW_HEIGHT - 50); // Ground level
    fuelCanSprite.setPosition(sf::Vector2f(x, y));
    fuelClock.restart();
}

void Player::spwanFuel(sf::RenderWindow &window)
{
    if(fuelClock.getElapsedTime().asSeconds() <= 10 && fuelClock.getElapsedTime().asSeconds() > 4)
    {
        window.draw(fuelCanSprite);
    }
    else if(fuelClock.getElapsedTime().asSeconds() >= 10)
    {
        fuelClock.restart();
    }
    else if(fuelClock.getElapsedTime().asSeconds() == 4)
    {
        setFuelCanPosition();
    }
}

void Player::fuelCanCollision()
{
    if(PlayerSprite.getGlobalBounds().intersects(fuelCanSprite.getGlobalBounds()))
    {
        fuelSound.play();
        fuelClock.restart();
        setFuelCanPosition();
        setFuel(200);
    }
}

void Player::setHumanoidCaptured(bool value)
{
    humanoidCaptured = value;
}

bool Player::isHumanoidCaptured() const
{
    return humanoidCaptured;
}

sf::FloatRect Player::getPlayerBounds() const
{
    return PlayerSprite.getGlobalBounds();
}

void Player::setPlayerState(bool playing) {
    isPlaying = playing;
    PlayerSprite.move(0, PLAYER_SPEED);
} 