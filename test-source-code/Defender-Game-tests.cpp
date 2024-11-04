#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.h"
#include "Player.h"
#include "Laser.h"
#include <SFML/Graphics.hpp>

TEST_CASE("Game is constructed and timer is initialised properly ") // this checks the initialisation of the timer based of the clock
{
    Game game;
    CHECK(game.frameClock.getElapsedTime().asSeconds() == doctest::Approx(1.0).epsilon(0.5)); // this tests that the timer works properly and the initial time value is lower than 1s with a tolerance of 0.5
    game.window.close();
}

// TEST_CASE("Generate splash screen") // this tests that the spash screen is correctly initialised
// // NB THIS TEST ONLY WORKS IF YOU CLOSE THE WINDOW AFTER RUNNING, IF YOU PLAY IT RETURNS FALSE
// {
//     Game game;
//     game.run(); // Run the game loop
// game.window.close();
//     CHECK(game.isSplashScreenDisplayed() == true); // Check if the flag is true when the splash screen is displayed
//     game.window.close();                           // this helps close the windows so you dont need to
// }

TEST_CASE("Game window size is correctly initialised ")
{
    Game game;
    sf::Vector2u expectedSize(WINDOW_WIDTH, WINDOW_HEIGHT); // This is the expected size

    sf::Vector2u actualSize = game.getWindowSize(); // this gets the actual window size

    CHECK(actualSize == expectedSize);
    game.window.close();
}

//////////////////////////////////////////////////LANDER TESTS///////////////////////////////////////////////////
TEST_CASE("Lander spawns within valid bounds") {
    Lander lander(0.0f);
    std::vector<sf::Vector2f> humanoidPositions; // You can pass an empty vector for this test

    // Spawn the Lander
    lander.spawnLander();

    // Get the Lander's position
    sf::Vector2f landerPosition = lander.getPosition();

    // Check if the Lander's position is within valid bounds
    CHECK(landerPosition.x >= 0.0f); // Assumes WINDOW_WIDTH is the right boundary
    CHECK(landerPosition.x <= WINDOW_WIDTH);
    CHECK(landerPosition.y >= 50.0f);
    CHECK(landerPosition.y <= WINDOW_HEIGHT - 100.0f);
}

// // this tests if the Lander has the texture of "landership.png"
TEST_CASE("Lander has the correct texture")
{
    Lander lander(1.0f); // this uses a cooldown of  1 second just for testing

    // this loads the texture manually for comparison
    sf::Texture expectedTexture;
    expectedTexture.loadFromFile("resources/landership.png");

    // this gets the actual texture from the Lander
    const sf::Texture &actualTexture = lander.getTexture();

    // this compares the textures
    CHECK(expectedTexture.getSize() == actualTexture.getSize());
}

TEST_CASE("Lander Moves When Game Runs")
{
    Lander lander(0.0f); // Initialize Lander with spawnCooldown of 0
    sf::Vector2f initialPosition = lander.getPosition();

    // Simulate game running for a certain time (e.g., 1 second)
    float deltaTime = 1.0f;
    sf::Vector2f playerPosition(100, 100); // Set player's position

    // Capture the initial position and update the Lander
    sf::Vector2f originalPosition = lander.getPosition();
    lander.update(deltaTime, playerPosition, std::vector<sf::Vector2f>());
    sf::Vector2f newPosition = lander.getPosition();

    // Check if the Lander's position has changed after updating
    CHECK(originalPosition != newPosition);
    
}

TEST_CASE("Lander gets Destroyed On Collision") {
    Lander lander(0.0f); // Initialise Lander with spawnCooldown of 0
    sf::Vector2f landerPosition(100.0f, 100.0f); // Set the initial Lander position
    Laser laser(landerPosition); // Create a Laser object for collision testing at the Lander's position
    lander.setPosition(landerPosition); // Set the Lander's position

    // Make sure the Lander is not initially destroyed
    CHECK_FALSE(lander.isDestroyed());

    // Check for collision and verify that the Lander gets destroyed
    bool collisionDetected = lander.checkCollision(laser);
    CHECK(collisionDetected);
    CHECK(lander.isDestroyed());
}

TEST_CASE("Lander can Fire Missiles") {
    Lander lander(0.0f); // Initialise Lander with spawnCooldown of 0
     CHECK(lander.missileCooldown.getElapsedTime().asSeconds() == doctest::Approx(0.0f).epsilon(0.01f)); //checks that the cooldown timer starts at zero
    // Initially, the Lander shouldnt be able to fire a missile as the cooldown is at zero
    CHECK_FALSE(lander.canFireMissile());
    //now the timer should be above zero, to test that the timer works, we call the simulated test
    CHECK(lander.canFireMissileSimulated());
          
}

TEST_CASE("Lander captures humanoid and detects state of humanoids destruction") {
    Lander lander(0.0f);
    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(100, 200, humanoidTexture);
    
    lander.setPosition(sf::Vector2f(100.0f, 100.0f)); // Set the Lander's position to the humanoid's position

    // Initially, Lander should not have captured a humanoid
    CHECK_FALSE(lander.humanoidIsCaptured());

    // Lander captures the humanoid
    lander.hasCapturedHumanoid();
    CHECK(lander.humanoidIsCaptured());

    // Lander should not detect destruction when humanoid is not destroyed
    REQUIRE(!humanoid.isDestroyed());
    CHECK_FALSE(lander.checkHumanoidDestroyed());
}



//////////////////////////MISSILETESTS///////////////////////////
TEST_CASE("Missile moves correctly")
{
    sf::Vector2f playerPosition(100, 100);
    Missile missile(50, 50, playerPosition);

    // this moves the missile
    missile.update(0.5f);

    // this checks if the missile's position has been updated correctly
    CHECK(missile.getBounds().left == doctest::Approx(50 + 0.35f * missile.speed).epsilon(1.0));
    CHECK(missile.getBounds().top == doctest::Approx(50 + 0.35f * missile.speed).epsilon(1.0));
}

TEST_CASE("Missile is out of bounds")
{
    sf::Vector2f playerPosition(100, 100);
    Missile missile(50, -10, playerPosition);

    // this checks if the missile is out of bounds
    CHECK(missile.isOutOfBounds() == true);

    missile.update(0.1f);

    // this checks if the missile is not out of bounds
    CHECK(missile.isOutOfBounds() == false);
}

TEST_CASE("Missile can hit and remove life from player")
{
    Player player;
    Missile missile(player.getPlayerPosition().x, player.getPlayerPosition().y, player.getPlayerPosition());

    int playerLives = 3;

    if(player.getPlayerBounds().intersects(missile.getBounds()))
    {
        playerLives--;
    }

    CHECK(playerLives == 2);
}

// ///////////////////////LASER///////////////////

TEST_CASE("Check Laser construction and initial position (moving right)")
{
    sf::Vector2f position(50, 50);
    bool reverseDirection = false;
    Laser laser(position, reverseDirection);

    // this checks the initial position of the laser
    CHECK(laser.getPosition().x == doctest::Approx(50));
    CHECK(laser.getPosition().y == doctest::Approx(50));
}

TEST_CASE("Laser moves correctly")
{
    Laser laser(sf::Vector2f(100, 100), false);
    CHECK(laser.shape.getPosition() == sf::Vector2f(100, 100));
    laser.move();
    CHECK(laser.shape.getPosition() != sf::Vector2f(100, 100));
}

TEST_CASE("Laser isOutOfBounds")
{
    Laser laser(sf::Vector2f(1600, 100), false);

    laser.shape.setPosition(sf::Vector2f(100, 100));
    CHECK(laser.isOutOfBounds(WINDOW_WIDTH) == false);
}
TEST_CASE("Check Laser construction and initial position (moving left)")
{
    sf::Vector2f position(60, 60);
    bool reverseDirection = true;
    Laser laser(position, reverseDirection);

    // this checks the initial position of the laser when moving left
    CHECK(laser.getPosition().x == doctest::Approx(60));
    CHECK(laser.getPosition().y == doctest::Approx(60 - laser.shape.getSize().y).epsilon(10.0));
}

TEST_CASE("Laser moves correctly (moving right)")
{
    sf::Vector2f position(70, 70);
    bool reverseDirection = false;
    Laser laser(position, reverseDirection);

    // this moves the laser
    laser.move();

    // this checks if the laser's position has been updated correctly
    CHECK(laser.getPosition().x == doctest::Approx(70 + LASER_SPEED).epsilon(25));
    CHECK(laser.getPosition().y == doctest::Approx(70));
}

/////////////////////////////////////////////////playertests//////////////////////////////////////

TEST_CASE("Check Player conforms to Constructor and initial states")
{
    Player player;
    // this checks the player's initial position

    // this checks if the game is not playing initially
    CHECK(player.isGamePlaying() == false);

    // this checks the laser cooldown timer
    CHECK(player.laserCooldownTimer != 0.0f);

    // this checks if the player's sprite has a texture
    CHECK(player.PlayerSprite.getTexture() != nullptr);
}

TEST_CASE("Player moves down when down button is pressed")
{
    Player player;

    // this Gets the initial position of the player
    sf::Vector2f initialPosition = player.getPlayerPosition();
    // this moves the player's position
    player.setPlayerState(true);

    // this gets the position after updating
    sf::Vector2f newPosition = player.getPlayerPosition();

    // this checks if the player's Y-coordinate increased (moved down)
    CHECK(newPosition.y > initialPosition.y);
}

////////////////////////////////////////////////////
TEST_CASE("Game Over Screen Displayed when called")
{
    Game game;
    game.gameOver = false; // Set gameOver to false
    // check if gameoverscreen is not displayed if game is not over
    CHECK(game.isGameOverScreenDisplayed == false);
    game.window.close();
    game.gameOver = true;
    game.showGameOverScreen();
    game.window.close();
    CHECK(game.isGameOverScreenDisplayed == true); // check that game over screen is in fact displayed now game is over
}

// ///////////////////minimaptests////////////////////////////////
TEST_CASE("Minimap exists and has a background")
{
    Game game;

    // this ensures that the minimap texture has a valid size
    CHECK(game.minimapTexture.getSize().x > 0);
    CHECK(game.minimapTexture.getSize().y > 0);
    game.spawnLander();

    // this ensures that the minimap background texture has a valid size
    CHECK(game.minimapBackgroundSprite.getTexture()->getSize().x > 0);
    CHECK(game.minimapBackgroundSprite.getTexture()->getSize().y > 0);
    game.window.close();
}

// /////////////////collisiontests//////////////////
TEST_CASE("Laser and Lander Collision Test")
{
    // this creates  a Lander and Laser
    Lander lander(0.0f);                     // this sets  a short spawn cooldown for testing
    Laser laser(lander.getPosition(), true); // this sets a laser that starts at the Lander's position
    sf::Vector2f playerPosition(100, 100); // Set player's position
    // this ensures the Lander is in a state where it can be destroyed
    lander.update(0.0f, sf::Vector2f(0.0f, 0.0f),std::vector<sf::Vector2f>() );
 
      // this checks initial destroyed state
    CHECK_FALSE(lander.isDestroyed());

    // this checks collision with a Lander
    bool collisionResult = lander.checkCollision(laser);
    CHECK(collisionResult);      // a collision should occur
    CHECK(lander.isDestroyed()); // the lander should be destroyed after collision
}

//////////////////////////////////////////////////////HIGHSCORE_TESTS///////////////////////////////////////
TEST_CASE("empty files can be loaded")
{
    HighScore highScoreManager;
    highScoreManager.clearHighScoresFile("highscores.txt");
    highScoreManager.loadFromFile("highscores.txt");
    CHECK(highScoreManager.getHighScores().empty());
}

// Test adding high scores in correct descending score order
TEST_CASE("AddHighScores are added and retrieved in correct descending order")
{
    HighScore highScoreManager;
    highScoreManager.clearHighScoresFile("highscores.txt");
    highScoreManager.loadFromFile("highscores.txt");
    highScoreManager.addHighScore("Alice", 500);
    highScoreManager.addHighScore("Bob", 300);
    highScoreManager.addHighScore("Mallory", 700);
    highScoreManager.saveToFile("highscores.txt");
    highScoreManager.loadFromFile("highscores.txt");
    auto highScores = highScoreManager.getHighScores();
    REQUIRE(highScores.size() == 3);
    CHECK(highScores[0].name == "Mallory");
    CHECK(highScores[0].score == 700);
    CHECK(highScores[1].name == "Alice");
    CHECK(highScores[1].score == 500);
    CHECK(highScores[2].name == "Bob");
    CHECK(highScores[2].score == 300);
}

// Test displaying high scores in correct descending order
TEST_CASE("DisplayHighScores displays high scores in correct descending order")
{
    HighScore highScoreManager;
    highScoreManager.loadFromFile("highscores.txt");
    auto highScores = highScoreManager.getHighScores();
    REQUIRE(highScores.size() == 3);
    CHECK(highScores[0].name == "Mallory");
    CHECK(highScores[1].name == "Alice");
    CHECK(highScores[2].name == "Bob");
    highScoreManager.clearHighScoresFile("highscores.txt"); // this ensures the
}

/////////////////////////////////////////////////////FUEL_SYSTEM_TESTS/////////////////////////////////////
TEST_CASE("Player Fuel quantity Decrements with movement ")
{
    Player player; // Create a Player instance
    CHECK(player.getFuel() == 200);
    // Check that initial fuel is 100
    float initialFuel = player.getFuel();
    player.moveLeft(); // moves player to the left, thereby decreasing the fuel
    float remainingFuel = player.getFuel();
    CHECK(remainingFuel == doctest::Approx(initialFuel - 0.1).epsilon(0.01)); // checks that remaining fuel is less than original fuel
}

TEST_CASE("player colliding with fuel can fills fuel to 100 ")
{
    Player player;
    player.setFuel(50);
    float initialfuel = player.getFuel();
    CHECK(initialfuel == doctest::Approx(50).epsilon(0.1));
    Player player2;
    player2.fuelCanCollision();
    float filledfuel = player2.getFuel();
    CHECK(filledfuel == doctest::Approx(200).epsilon(0.1));
}

TEST_CASE("fuel bar size decreases with player fuel content")
{
    Player player;
    player.setFuel(100);
    player.fuelBar.setSize(sf::Vector2f(player.getFuel(), 10));
    float initiallength = player.fuelBar.getSize().x;
    player.setFuel(50);
    player.fuelBar.setSize(sf::Vector2f(player.getFuel(), 10));
    float finallength = player.fuelBar.getSize().x;
    CHECK(initiallength > finallength);
}

TEST_CASE("fuel can is spawned on the screen at at the correct position")
{
    Player player; // Create a player instance
    player.setFuelCanPosition();
    CHECK(player.fuelCanSprite.getPosition().y >= 0.0f);
    CHECK(player.fuelCanSprite.getPosition().x <= 1500);
}
///////////////////////////////////////////////////////HUMANOID_TESTS//////////////////////////////////////////////////////
TEST_CASE("Humanoid is spawned correctly") {
    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(100, 200, humanoidTexture);

    CHECK(humanoid.getPosition() == sf::Vector2f(100, 200));
    CHECK(humanoid.isCaptured() == false);
    CHECK(humanoid.isFalling() == false);
    CHECK(humanoid.isDestroyed() == false);
}

TEST_CASE("Humanoid moves correctly") {
    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(100, 200, humanoidTexture);

    int numOfHumanoids = 5;

    // Test the initial position
    CHECK(humanoid.getPosition() == sf::Vector2f(100, 200));

    // Move the humanoid
    humanoid.update(numOfHumanoids);

    // Check if the position has changed (moves right initially)
    CHECK(humanoid.getPosition() != sf::Vector2f(100, 200));

    // Capture the humanoid
    humanoid.capture(sf::Vector2f(300, 400));

    // Check if it's captured and not falling
    CHECK(humanoid.isCaptured() == true);
    CHECK(humanoid.isFalling() == false);

    // Release the humanoid
    humanoid.release();

    // Check if it's released and falling
    CHECK(humanoid.isCaptured() == false);
    CHECK(humanoid.isFalling() == true);
}

TEST_CASE("Humanoid can be deposited") {
    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(100, 200, humanoidTexture);

    // Capture and then deposit the humanoid
    humanoid.capture(sf::Vector2f(300, 400));
    humanoid.deposit(500);

    // Check if it's not captured, not falling, and in the correct position
    CHECK(humanoid.isCaptured() == false);
    CHECK(humanoid.isFalling() == false);
    CHECK(humanoid.getPosition().x == doctest::Approx(500).epsilon(50));
}

TEST_CASE("Humanoid can be captured by player") {
    Player player;

    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(player.getPlayerPosition().x, player.getPlayerPosition().y, humanoidTexture);

    int numOfHumanoids = 5;
    humanoid.startFalling();

    if(player.getPlayerBounds().intersects(humanoid.getBounds()) && humanoid.isFalling() && humanoid.getPosition().y != WINDOW_HEIGHT-100) //&& !player.isHumanoidCaptured())
    {
        humanoid.capture(player.getPlayerPosition());
        player.setHumanoidCaptured(true);
        humanoid.setPlayerCaptured(true);
        humanoid.update(numOfHumanoids);
    }

    CHECK(humanoid.isCaptured() == true);
    CHECK(humanoid.isFalling() == false);
    CHECK(humanoid.getPosition() == player.getPlayerPosition());
    CHECK(humanoid.isPlayerCaptured() == true);
    CHECK(player.isHumanoidCaptured() == true);
}

TEST_CASE("Humanoid can be deposited by player") {
    Player player;
    player.PlayerSprite.setPosition(player.getPlayerPosition().x, WINDOW_HEIGHT-100);

    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(player.getPlayerPosition().x, player.getPlayerPosition().y, humanoidTexture);

    int numOfHumanoids = 5;
    humanoid.setPlayerCaptured(true);
    player.setHumanoidCaptured(true);

    if(player.getPlayerPosition().y >= WINDOW_HEIGHT-100 && player.isHumanoidCaptured() && humanoid.isPlayerCaptured())
    {
        humanoid.deposit(player.getPlayerPosition().x);
        player.setHumanoidCaptured(false);
        humanoid.setPlayerCaptured(false);
        humanoid.update(numOfHumanoids);
    }

    CHECK(humanoid.isCaptured() == false);
    CHECK(humanoid.isFalling() == false);
    CHECK(humanoid.getPosition().y ==  WINDOW_HEIGHT-100);
    CHECK(humanoid.isPlayerCaptured() == false);
    CHECK(player.isHumanoidCaptured() == false);
}

TEST_CASE("Humanoid can be killed")
{
    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(100, 200, humanoidTexture);

    Laser laser(humanoid.getPosition(), true); // this sets a laser that starts at the Lander's position

    if(humanoid.getBounds().intersects(laser.getBounds()))
    {
        humanoid.setDestroy();
    }

    CHECK(humanoid.isDestroyed() == true);
}

TEST_CASE("Humanoid is killed when its captured by lander and lander reaches top of window")
{
    sf::Texture humanoidTexture;
    humanoidTexture.loadFromFile("resources/landership.png");
    
    Humanoid humanoid(100, 60, humanoidTexture);

    Lander lander(0.0f); 

    lander.setPosition(humanoid.getPosition());

    humanoid.capture(lander.getPosition());
    lander.hasCapturedHumanoid();

    int numOfHumanoids = 5;

    humanoid.update(numOfHumanoids);

    CHECK(humanoid.isDestroyed() == true);
    CHECK(numOfHumanoids == 4);
}

////////////////////////////BACKGROUND_SCROLLING_TESTS//////////////
TEST_CASE("Background scrolls when player moves left and right")
{
    Game game;
    Player player;
    float initialBackgroundPosition = game.backgroundSprite.getPosition().x;
    float finalBackgroundPosition = game.backgroundPosition.x = 5.0f;
    CHECK(finalBackgroundPosition != initialBackgroundPosition);
}

//////////////////////////Game display and Logic//////////////////////////////////'
// All test cases below work, but require manual closing of windows
// TEST_CASE("Game won when all landers destroyed")
// {
//     Game game;
    
//     int landersDestroyed = 10;

//     if(landersDestroyed >= 10)
//     {
//         game.setGameWon();
//         game.showGameOverScreen();
//     }

//     CHECK(game.getGamewon() == true);
//     CHECK(game.getIsGameOverScreenDisplayed() == true);

//     //game.window.clear();
//     game.window.close();
// }

// TEST_CASE("Game over when player dies")
// {
//     Game game;
    
//     int playerLives = 0;

//     if(playerLives <= 0)
//     {
//         game.showGameOverScreen();
//     }

//     CHECK(game.getGamewon() == false);
//     CHECK(game.getIsGameOverScreenDisplayed() == true);

//     //game.window.clear();
//     game.window.close();
// }

// TEST_CASE("Game over when all Humanoids die")
// {
//     Game game;
    
//     int numHumanoids = 0;

//     if(numHumanoids <= 0)
//     {
//         game.showGameOverScreen();
//     }

//     CHECK(game.getGamewon() == false);
//     CHECK(game.getIsGameOverScreenDisplayed() == true);

//     //game.window.clear();
//     game.window.close();
// }