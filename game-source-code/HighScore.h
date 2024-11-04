#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

/**
 * @class HighScore
 * @brief Represents high scores in the game.
 *
 * The HighScore class manages high score data and provides functions to load, save,
 * add, and display high scores in the game.
 */
class HighScore
{
public:
    /**
     * @brief Default constructor for the HighScore class.
     */
    HighScore();

    // Load high scores from a file
    /**
     * @brief Load high scores from a file.
     *
     * @param filename The name of the file from which to load high scores.
     */
    void loadFromFile(const std::string &filename);

    // Save high scores to a file
    /**
     * @brief Save high scores to a file.
     *
     * @param filename The name of the file to which high scores will be saved.
     */
    void saveToFile(const std::string &filename);

    // Add a new high score
    /**
     * @brief Add a new high score.
     *
     * @param name The name associated with the high score.
     * @param score The high score value to be added.
     */
    void addHighScore(const std::string &name, int score);

    // Display high scores
    // void displayHighScores();

    /**
     * @brief Display high scores on the specified window.
     *
     * @param window The SFML render window where high scores will be displayed.
     */
    void displayHighScores(sf::RenderWindow &window);

    /**
     * @brief Clear the high scores file as needed in the tests.
     *
     * @param filename The name of the high scores file to clear.
     */
    void clearHighScoresFile(const std::string &filename); // this clears the high score file as needed in the tests

    /**
     * @struct ScoreEntry
     * @brief Represents a high score entry with a name and score.
     */
    struct ScoreEntry
    {
        std::string name;
        int score;
    };
    std::vector<ScoreEntry> scores;

    /**
     * @brief Get the high scores as a vector of ScoreEntry objects.
     *
     * @return A vector of ScoreEntry objects representing the high scores.
     */
    std::vector<ScoreEntry> getHighScores() const; // this is also for test purposes
private:
};