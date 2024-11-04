#include "Game.h"
#include "Player.h"
#include "Laser.h"
#include "Lander.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "Humanoid.h"

const float BACKGROUND_SCROLL_SPEED = 500.0f;
const float LANDER_SPAWN_COOLDOWN = 1.5f;
const int INITIAL_NUM_LIVES = 3;
const int SHIELD_EFFECT_LENGTH = 5.0f;
const int MINIMAP_WIDTH = 100.0f;
const int MINIMAP_HEIGHT = 60.0f;
const int BORDER_SIZE = 5.0f;
const int LANDER_HEIGHT = 30.0f;
const int MOVEMENT_SPEED = 2.0f;
const int HUMANOID_HEIGHT = 30.0f;

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Space Defender", sf::Style::Titlebar | sf::Style::Close), splashScreenDisplayed(false), spawnTimer(), lander(LANDER_SPAWN_COOLDOWN), score(0), numLives(3), numShields(3), numHumanoids(5), gameOver(false), shieldFrame(sf::Vector2f(player.getPlayerBounds().width + 10, player.getPlayerBounds().height + 10)),
      shieldOn(false), isGameOverScreenDisplayed(false), gameWon(false), totalLandersSpawned(0), numLandersDestroyed(0), numHumanoidsInTotal(0), allHumanoidsDead(false), highScoreManager(), typingName(false)
{
    shieldFrame.setOutlineThickness(5);
    shieldFrame.setOutlineColor(sf::Color::Blue);
    shieldFrame.setFillColor(sf::Color::Transparent);

    newGameTimer.restart();
    frameClock.restart();
    window.setFramerateLimit(60);
    if (!font.loadFromFile("resources/INVASION2000.ttf"))
    {
        std::cerr << "Failed to load font file" << std::endl;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    fuelText.setFont(font);
    fuelText.setCharacterSize(20);
    fuelText.setFillColor(sf::Color::White);
    fuelText.setPosition(WINDOW_WIDTH - 450, 5);

    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(10, 40);

    shieldsText.setFont(font);
    shieldsText.setCharacterSize(24);
    shieldsText.setFillColor(sf::Color::White);
    shieldsText.setPosition(10, 70);

    humanoidText.setFont(font);
    humanoidText.setCharacterSize(24);
    humanoidText.setFillColor(sf::Color::White);
    humanoidText.setPosition(10, 100);

    if (!backgroundTexture.loadFromFile("resources/space4.jpg"))
    {
        std::cerr << "Failed to load sidescroll.jpg" << std::endl;
    }

    if (!explosionBuffer.loadFromFile("resources/explosion.wav")) // the following are resources
    {
        std::cerr << "Failed to load gun.wav" << std::endl;
    }
    explosionSound.setBuffer(explosionBuffer);

    if (!shieldBuffer.loadFromFile("resources/shield.mp3")) // the following are resources
    {
        std::cerr << "Failed to load gun.wav" << std::endl;
    }
    shieldSound.setBuffer(shieldBuffer);

    if (!crashBuffer.loadFromFile("resources/player_hit.mp3")) // the following are resources
    {
        std::cerr << "Failed to load gun.wav" << std::endl;
    }
    crashSound.setBuffer(crashBuffer);

    if (!HumanoidBuffer.loadFromFile("resources/humanoid_dead.wav")) // the following are resources
    {
        std::cerr << "Failed to load humanoid_dead.wav" << std::endl;
    }
    HumanoidSound.setBuffer(HumanoidBuffer);

    if (!humanoidTexture.loadFromFile("resources/humanoid.png"))
    {
        std::cerr << "Failed to load humanoid texture!" << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(WINDOW_WIDTH * 3) / backgroundTexture.getSize().x,
                              static_cast<float>(WINDOW_HEIGHT) / backgroundTexture.getSize().y);

    backgroundPosition = sf::Vector2f(0, 0);
    minimapTexture.clear(sf::Color::Black);
    minimapTexture.create(MINIMAP_WIDTH, MINIMAP_HEIGHT);

    if (!minimapBackgroundTexture.loadFromFile("resources/space4.jpg"))
    {
        std::cerr << "Failed to load pixelminimap.png" << std::endl;
    }
    minimapBackgroundSprite.setTexture(minimapBackgroundTexture);
    minimapBackgroundSprite.setScale(static_cast<float>(MINIMAP_WIDTH) / minimapBackgroundTexture.getSize().x,
                                     static_cast<float>(MINIMAP_HEIGHT) / minimapBackgroundTexture.getSize().y);
}

void Game::updateScoreboard()
{
    // this updates the text for score, lives, and shields
    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + (numLives > 0 ? std::to_string(numLives) : "DEAD"));
    shieldsText.setString("Shields: " + std::to_string(numShields));
    humanoidText.setString("Humanoids Alive: " + std::to_string(numHumanoids));
    fuelText.setString("Fuel Remaining");
}

void Game::run()
{
    while (window.isOpen())
    {
        sf::Time frameTime = frameClock.restart();
        float deltaTime = frameTime.asSeconds();

        player.handleInput(window, lasers);
        minimapTexture.clear(sf::Color::Black);
        // this calculates the maximum allowable background position
        float maxBackgroundX = WINDOW_WIDTH - backgroundSprite.getGlobalBounds().width;
        // Update and draw humanoids

        // this limits the background scrolling to the left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && backgroundPosition.x < 0)
        {
            backgroundPosition.x += BACKGROUND_SCROLL_SPEED * deltaTime;
        }

        // this limits the background scrolling to the right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && backgroundPosition.x > -WINDOW_WIDTH - 100)
        {
            backgroundPosition.x -= BACKGROUND_SCROLL_SPEED * deltaTime;
        }

        backgroundSprite.setPosition(backgroundPosition);
        window.clear();
        window.draw(backgroundSprite);

        if (isGameActive)
        {

            // this checks if it's time to update the player dot on the minimap
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && backgroundPosition.x < 0)
            {
                backgroundPosition.x += BACKGROUND_SCROLL_SPEED * deltaTime;
            }

            // this limits the background scrolling to the right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && backgroundPosition.x > -WINDOW_WIDTH - 100)
            {
                backgroundPosition.x -= BACKGROUND_SCROLL_SPEED * deltaTime;
            }
            sf::Vector2f scaleFactor(
                static_cast<float>(backgroundTexture.getSize().x) / static_cast<float>(MINIMAP_WIDTH / 20),
                static_cast<float>(backgroundTexture.getSize().y) / static_cast<float>(MINIMAP_HEIGHT / 10));

            // this updates the scrolling background and minimap background position
            backgroundSprite.setPosition(backgroundPosition);
            minimapBackgroundSprite.setPosition(backgroundPosition.x / scaleFactor.x, backgroundPosition.y / scaleFactor.y);

            sf::RectangleShape minimapBorder(sf::Vector2f(MINIMAP_WIDTH * 3 + BORDER_SIZE * 2, MINIMAP_HEIGHT * 2 + BORDER_SIZE * 2));
            minimapBorder.setFillColor(sf::Color::Transparent);
            minimapBorder.setOutlineThickness(BORDER_SIZE);  // this sets the border thickness
            minimapBorder.setOutlineColor(sf::Color::White); // this Sets the border color

            float maxBackgroundX = WINDOW_WIDTH - backgroundSprite.getGlobalBounds().width;

            // this calculates the player dot's position on the minimap
            float playerDotX = player.getPlayerPosition().x * (MINIMAP_WIDTH / static_cast<float>(WINDOW_WIDTH));
            float playerDotY = player.getPlayerPosition().y * (MINIMAP_HEIGHT / static_cast<float>(WINDOW_HEIGHT));

            sf::CircleShape playerDot(2.0f); // this creates a small dot for the player on the mini map
            playerDot.setFillColor(sf::Color::Blue);
            playerDot.setPosition(playerDotX, playerDotY);
            spawnHumanoids();

            // this draws the minimap at the top of the screen
            sf::Sprite minimapSprite(minimapTexture.getTexture());
            minimapTexture.draw(minimapBackgroundSprite);
            minimapSprite.setPosition(static_cast<float>(WINDOW_WIDTH) - MINIMAP_WIDTH - 1000.0f, 10.0f);
            minimapSprite.setScale(3.0f, 2.0f);
            minimapBorder.setPosition(minimapSprite.getPosition().x - BORDER_SIZE, minimapSprite.getPosition().y - BORDER_SIZE);
            window.draw(minimapBorder);
            // this draws the player dot on the minimap
            minimapTexture.draw(playerDot);

            // Update and draw humanoids

            for (const auto &lander : landers)
            {
                if (!lander.isDestroyed())
                {

                    float landerDotX = lander.getPosition().x * (MINIMAP_WIDTH / static_cast<float>(WINDOW_WIDTH));
                    float landerDotY = lander.getPosition().y * (MINIMAP_HEIGHT / static_cast<float>(WINDOW_HEIGHT));

                    sf::CircleShape landerDot(2.0f); // this creates a small dot for the Lander
                    landerDot.setFillColor(sf::Color::Yellow);
                    landerDot.setPosition(landerDotX, landerDotY);
                    // this draws the Lander dot on the minimap
                    minimapTexture.draw(landerDot);
                }
            }

            for (const auto &humanoid : humanoids)
            {
                if (!humanoid.isDestroyed())
                {
                    // Calculate the position of the humanoid dot on the minimap
                    float humanoidDotX = humanoid.getPosition().x * (MINIMAP_WIDTH / static_cast<float>(WINDOW_WIDTH));
                    float humanoidDotY = humanoid.getPosition().y * (MINIMAP_HEIGHT / static_cast<float>(WINDOW_HEIGHT));

                    sf::CircleShape humanoidDot(2.0f);
                    humanoidDot.setFillColor(sf::Color::Green); // You can choose the color you like
                    humanoidDot.setPosition(humanoidDotX, humanoidDotY);

                    // Draw the humanoid dot on the minimap
                    minimapTexture.draw(humanoidDot);
                }
            }

            // Inside your game loop
            // Inside your game loop

            window.draw(minimapBackgroundSprite);
            window.draw(minimapSprite);

            minimapTexture.display();
        }

        updateScoreboard();

        window.draw(scoreText);
        window.draw(fuelText);
        window.draw(livesText);
        window.draw(shieldsText);
        window.draw(humanoidText);
        // this updates and draw Landers
        for (auto &lander : landers)
        {
            lander.update(deltaTime, player.getPlayerPosition(), humanoidPositions);

            if (lander.isDestroyed())
            {
                lander.draw(window);
            }
        }

        if (numLandersDestroyed > 10 && numLives!=0)
        {
            gameWon = true;
            showGameOverScreen();
        }

        if (player.isGamePlaying())
        {
            player.update(lasers);

            spawnHumanoids();

            updateHumanoids();

            // Find the part where you create a new Lander object
            if (spawnTimer.getElapsedTime().asSeconds() >= LANDER_SPAWN_COOLDOWN) // landers.size() < 5 &&
            {
                spawnLander();
                spawnTimer.restart();
            }

            for (auto &laser : lasers)
            {
                // change this code in a bit
                for (Humanoid &humanoid : humanoids)
                {
                    // Check if the laser intersects with the humanoid's bounds
                    if (laser.shape.getGlobalBounds().intersects(humanoid.getBounds()) && !humanoid.isCaptured() && !laser.isDestroyed() && !humanoid.isDestroyed() && (humanoid.isFalling() || humanoid.getPosition().y == WINDOW_HEIGHT - 100))
                    {
                        score -= 50;
                        // Handle collision logic for humanoid here

                        // Mark the humanoid for removal
                        humanoid.setDestroy();
                        HumanoidSound.play();
                        numHumanoids--;
                        laser.setDestroyed();
                    }
                }
            }

            for (auto &laser : lasers)
            {
                if (!laser.isDestroyed())
                {
                    window.draw(laser.shape);
                }

                for (auto &lander : landers)
                {
                    if (lander.checkCollision(laser))
                    {
                        explosionSound.play();
                        score += 50;
                        lander.setDestroyed(); // true);
                        numLandersDestroyed++;
                        laser.setDestroyed();
                    }
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && shieldCooldown.getElapsedTime().asSeconds() >= SHIELD_EFFECT_LENGTH)
            {
                if (numShields > 0)
                {
                    shieldCooldown.restart();
                    shieldOn = true;
                    shieldSound.play();
                    numShields--;
                }
            }
            if (shieldOn)
            {

                sf::Vector2f playerPosition = player.getPlayerPosition();
                shieldFrame.setPosition(playerPosition);
                // this sets the shield frame size to match the player's ship size
                shieldFrame.setSize(sf::Vector2f(player.getPlayerBounds().width + 10, player.getPlayerBounds().height + 10));
                // this flips the shield frame when the player changes directions
                if (!player.isFacingRight)
                {
                    shieldFrame.setScale(-1.0f, 1.0f);
                }
                else
                {
                    shieldFrame.setScale(1.0f, 1.0f);
                }
                window.draw(shieldFrame);
            }
            // this checks if the shield is still active and apply its effects
            if (shieldOn && shieldCooldown.getElapsedTime().asSeconds() >= 5.0f)
            {

                shieldOn = false;
            }

            if (missileSpawnTimer.getElapsedTime().asSeconds() > 5.0f)
            {
                spawnMissilesFromLanders();
                missileSpawnTimer.restart();
            }

            player.spwanFuel(window);
            player.fuelCanCollision();

            checkPlayerHumanoidCollision();

            if (player.getFuel() <= 0)
            {

                // Player has run out of fuel, trigger game over
                while (player.getPlayerPosition().y <= WINDOW_HEIGHT - 20)
                {
                    player.PlayerSprite.move(0, PLAYER_SPEED);
                    window.clear();
                    window.draw(backgroundSprite);
                    player.draw(window);
                    window.display();
                }
                showGameOverScreen();
            }

            for (auto &lander : landers)
            {
                checkLanderHumanoidCollisions(lander);
                if (!lander.isDestroyed())
                {
                    lander.update(deltaTime, player.getPlayerPosition(), humanoidPositions);

                    // this checks for collision between player and lander
                    if (player.getPlayerBounds().intersects(lander.landerSprite.getGlobalBounds()) && !shieldOn && intersectionCollisionTimer.getElapsedTime().asSeconds() >= 2.0f)
                    {
                        crashSound.play();
                        intersectionCollisionTimer.restart();

                        numLives--;
                        numLandersDestroyed++;
                        if (numLives <= 0)
                        {
                            newGameTimer.restart();
                            gameOver = true;
                            gameWon = false;
                        }

                        if (gameOver)
                        {
                            showGameOverScreen();
                        }
                        else if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::N) && newGameTimer.getElapsedTime().asSeconds() >= 5.0f)
                        {
                            // this resets the game if the player presses 'N'

                            showGameOverScreen();
                            resetGame();
                        }
                        lander.setDestroyed();
                    }
                    else
                    {
                        if (!lander.isDestroyed())
                        {
                            lander.draw(window);
                        }
                    }
                }
            }

            for (auto it = missiles.begin(); it != missiles.end();)
            {
                it->update(deltaTime);

                sf::FloatRect missileBounds = it->getBounds();
                sf::FloatRect landerBounds = lander.getLanderBounds();
                sf::FloatRect playerBounds = player.getPlayerBounds();

                if (missileBounds.intersects(playerBounds) && !shieldOn && collisionTimer.getElapsedTime().asSeconds() >= 1.5f)
                {
                    crashSound.play();
                    collisionTimer.restart();
                    numLives--;
                    if (numLives <= 0)
                    {
                        newGameTimer.restart();
                        gameOver = true;
                    }
                    if (gameOver)
                    {
                        showGameOverScreen();
                    }
                    else if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::N) && newGameTimer.getElapsedTime().asSeconds() >= 5.0f)
                    {
                        // this resets the game if the player presses space
                        showGameOverScreen();

                        resetGame();
                    }
                    it = missiles.erase(it);
                }
                else if (missileBounds.top + missileBounds.height < 0)
                {
                    it = missiles.erase(it);
                }
                else
                {
                    it->draw(window);
                    ++it;
                }
            }

            player.draw(window);
            drawHumanoids();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                isGameActive = true;
                player.startGame();
                splashScreenDisplayed = false;
            }
            lander.update(deltaTime, player.getPlayerPosition(), humanoidPositions);
        }
        else
        {
            drawSplashScreen();
            splashScreenDisplayed = true;
        }
        window.display();
    }
}

void Game::drawSplashScreen()
{
    window.clear();

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("resources/8bitspace.jpg"))
    {
        std::cerr << "Failed to load background image" << std::endl;
        return;
    }
    sf::Sprite backgroundImage;
    backgroundImage.setTexture(backgroundTexture);
    backgroundImage.setScale(3, 2.5);

    // Set the window to use the view of the splash screen (optional)
    window.setView(window.getDefaultView());

    // Draw the background image
    window.draw(backgroundImage);
    sf::Font font;
    if (!font.loadFromFile("resources/INVASION2000.ttf"))
    {
        std::cerr << "Failed to load font file" << std::endl;
        return;
    }

    sf::Text text("SPACE DEFENDER\n\nPress SPACE to play\nPress ESC to exit \n \n HOW TO PLAY: \n PRESS ARROW KEYS TO MOVE \n PRESS SPACEBAR TO SHOOT \n PRESS Q FOR SHIELD", font, 60);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(300, 300);

    window.draw(text);
}

bool Game::isSplashScreenDisplayed() const
{
    return splashScreenDisplayed;
}

sf::Vector2u Game::getWindowSize() const
{
    return window.getSize();
}

void Game::spawnLander()
{
    if (spawnTimer.getElapsedTime().asSeconds() >= LANDER_SPAWN_COOLDOWN && totalLandersSpawned <= 10)
    {
        // this creates a new Lander dynamically and add it to the vector using smart pointers
        Lander *newLander = new Lander(LANDER_SPAWN_COOLDOWN);
        landers.emplace_back(*newLander);
        activeLander = newLander; // this sets the active lander pointer to the new lander
        spawnTimer.restart();

        // Increment the total number of landers spawned
        totalLandersSpawned++;
    }
}

void Game::spawnMissilesFromLanders()
{
    for (auto &lander : landers)
    {
        if (!lander.isDestroyed())
        {
            sf::Vector2f landerPosition = lander.landerSprite.getPosition();
            sf::Vector2f playerPosition = player.getPlayerPosition();
            // this creates a new missile with the player's position as the target
            missiles.emplace_back(Missile(landerPosition.x, landerPosition.y, playerPosition));
        }
    }
}

void Game::showGameOverScreen()
{
    isGameOverScreenDisplayed = true;
    sf::Font font;
    if (!font.loadFromFile("resources/INVASION2000.ttf"))
    {
        std::cerr << "Failed to load font file" << std::endl;
        return;
    }
    sf::Font font2;
    if (!font2.loadFromFile("resources/sansation.ttf"))
    {
        std::cerr << "Failed to load font2 file" << std::endl;
        return;
    }

    sf::Text winText("You Win!", font, 60);
    winText.setFillColor(sf::Color::Green);
    winText.setStyle(sf::Text::Bold);
    winText.setPosition((WINDOW_WIDTH / 2) - 200, (WINDOW_HEIGHT / 2) - 200);

    sf::Text HumanoidDeadGameOverText("ALL HUMANOIDS WERE KILLED!!", font, 60);
    HumanoidDeadGameOverText.setFillColor(sf::Color::Red);
    HumanoidDeadGameOverText.setStyle(sf::Text::Bold);
    HumanoidDeadGameOverText.setPosition((WINDOW_WIDTH / 2) - 600, (WINDOW_HEIGHT / 2) - 200);

    sf::Text gameOverText("Game Over", font, 60);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2);

    sf::Text scoreText("Score: " + std::to_string(score), font, 40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 + 50.0f);

    sf::Text playAgainText("Press N to play again", font, 30);
    playAgainText.setFillColor(sf::Color::White);
    playAgainText.setPosition(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 + 100.0f);

    sf::Text quitText("Press ESC to quit", font, 30);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 + 150.0f);

    sf::Text promptText("Press G to add your score to the database", font, 20);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 + 200.0f);

    sf::Text nameInputText("Enter your name:", font, 20);
    nameInputText.setFillColor(sf::Color::White);
    nameInputText.setPosition(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 + 250.0f);

    sf::RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setFillColor(sf::Color::Black);
    inputBox.setOutlineColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 + 280.0f);

    sf::Text playerNameText("", font2, 20);
    // Set the loaded font

    playerNameText.setFillColor(sf::Color::White);
    playerNameText.setPosition(WINDOW_WIDTH / 2 - 295, WINDOW_HEIGHT / 2 + 285.0f);

    bool scoreAdded = false;
    bool nameEntered = false;
    std::string playerName;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::N && !typingName)
                {
                    resetGame();
                    return;
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return;
                }
                else if (event.key.code == sf::Keyboard::G && !scoreAdded && !typingName)
                {
                    // Prompt the player to enter their name
                    nameEntered = true;
                    typingName = true;
                }
                else if (nameEntered && event.key.code == sf::Keyboard::Return)
                {
                    // Player has finished entering their name
                    playerName = playerNameText.getString();
                    nameEntered = false;
                    scoreAdded = true;
                    typingName = false;

                    // Add the player's score to the high scores
                    highScoreManager.addHighScore(playerName, score);
                }
                else if (nameEntered && event.key.code != sf::Keyboard::Return)
                {
                    // Handle text input for player's name
                    if (event.text.unicode < 27) // Allow ASCII characters
                    {
                        playerName += static_cast<char>(event.text.unicode + 65); // no idea why?
                        playerNameText.setString(playerName);
                    }
                }
            }
        }

        window.clear();
        if (allHumanoidsDead)
        {
            window.draw(HumanoidDeadGameOverText);
        }
        if (!gameWon)
        {
            window.draw(gameOverText);
        }
        else
        {
            window.draw(winText);
        }
        window.draw(scoreText);
        window.draw(playAgainText);
        window.draw(quitText);
        window.draw(promptText);

        if (nameEntered)
        {
            window.draw(nameInputText);
            window.draw(inputBox);
            window.draw(playerNameText);
        }

        // Display high scores
        highScoreManager.displayHighScores(window);

        window.display();
    }
}

void Game::resetGame()
{
    // this resets game-related variables to their initial values
    score = 0;
    numLives = INITIAL_NUM_LIVES;
    numShields = 3;
    numHumanoids = 5;
    allHumanoidsDead = 0;

    numHumanoidsInTotal = 0;
    totalLandersSpawned = 0;
    numLandersDestroyed = 0;
    player.PlayerSprite.setPosition(WINDOW_WIDTH / 2 - player.PlayerSprite.getLocalBounds().width / 2,
                                    WINDOW_HEIGHT / 2 - player.PlayerSprite.getLocalBounds().height / 2);
    player.setFuel(100);
    isGameOverScreenDisplayed = false;

    missiles.clear();
    gameOver = false;
    // this clears the landers and lasers vectors
    humanoids.clear();
    landers.clear();
    lasers.clear();
    shieldOn = false;
    player.PlayerSprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    run();
}

void Game::spawnHumanoids()
{
    if (humanoids.size() < 5 && numHumanoidsInTotal <= 4) // Limit the total number of humanoids
    {
        // Generate a random position at the bottom of the screen
        float x = static_cast<float>(rand() % WINDOW_WIDTH);
        float y = static_cast<float>(WINDOW_HEIGHT - 100);

        // Create a new humanoid and set its position
        Humanoid newHumanoid(x, y, humanoidTexture);
        // sf::Vector2f humanoidPosition = sf::Vector2f(x, y);
        // humanoidPositions.push_back(humanoidPosition);
        humanoidPositions.emplace_back(x, y);
        // Add the humanoid to the vector of humanoids
        humanoids.push_back(newHumanoid);
        numHumanoidsInTotal++;
    }
}

void Game::updateHumanoids()
{
    // Iterate through your humanoid objects and update their positions
    int i = 0;
    for (Humanoid &humanoid : humanoids)
    {
        // Assuming your Humanoid class has a function to update its position, e.g., updatePosition()
        humanoid.update(numHumanoids); // You should implement this function in your Humanoid class
        humanoidPositions[i] = humanoid.getPosition();
        if (numHumanoids <= 0)
        {
            allHumanoidsDead = true;
            gameOver = true;
            gameWon = false;
            showGameOverScreen();
        }
        i++;
    }
}

void Game::drawHumanoids()
{
    for (Humanoid &humanoid : humanoids)
    {
        if (!humanoid.isDestroyed())
        {
            humanoid.draw(window);
        }
    }
}

void Game::checkLanderHumanoidCollisions(Lander &lander)
{
    for (Humanoid &humanoid : humanoids)
    {
        if (humanoid.getBounds().intersects(lander.landerSprite.getGlobalBounds())) //&& !lander.humanoidIsCaptured())
        {
            humanoid.capture(lander.getPosition());
            lander.hasCapturedHumanoid();
        }
        else if (lander.humanoidIsCaptured() && humanoid.isCaptured() && !lander.isDestroyed())
        {
            humanoid.capture(lander.getPosition());
        }
        
        else if (lander.isDestroyed() && humanoid.isCaptured())
        {
            humanoid.release();
        }
    }
}

void Game::checkPlayerHumanoidCollision()
{
    for (Humanoid &humanoid : humanoids)
    {
        if (player.getPlayerBounds().intersects(humanoid.getBounds()) && humanoid.isFalling() && humanoid.getPosition().y != WINDOW_HEIGHT - 50) //&& !player.isHumanoidCaptured())
        {
            humanoid.capture(player.getPlayerPosition());
            player.setHumanoidCaptured(true);
            humanoid.setPlayerCaptured(true);
            humanoid.update(numHumanoids);
        }
        else if (player.getPlayerPosition().y >= WINDOW_HEIGHT - 200 && player.isHumanoidCaptured() && humanoid.isPlayerCaptured())
        {
            humanoid.deposit(player.getPlayerPosition().x);
            player.setHumanoidCaptured(false);
            humanoid.setPlayerCaptured(false);
            humanoid.update(numHumanoids);
        }
    }
}

int Game::getNumLandersDestroyed()
{
    return numLandersDestroyed;
}

void Game::setGameWon()
{
    gameWon = true;
}

bool Game::getGamewon()
{
    return gameWon;
}

bool Game::getIsGameOverScreenDisplayed()
{
    return isGameOverScreenDisplayed;
}

Game::~Game()
{
    // this is the default destructor for Game
}