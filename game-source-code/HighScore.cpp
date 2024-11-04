
#include "HighScore.h"
#include "game.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

HighScore::HighScore()
{
    // Load high scores from a file (you can specify the filename)
    loadFromFile("highscores.txt");
}

void HighScore::loadFromFile(const std::string &filename)
{
    // Load high scores from the file and populate the scores vector
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open high scores file." << std::endl;
        return;
    }

    scores.clear();

    std::string name;
    int score;
    while (file >> name >> score)
    {
        scores.push_back({name, score});
    }

    file.close();
}

void HighScore::saveToFile(const std::string &filename)
{
    // Save high scores to the file
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open high scores file for writing." << std::endl;
        return;
    }

    for (const auto &entry : scores)
    {
        file << entry.name << " " << entry.score << "\n";
    }

    file.close();
}

void HighScore::addHighScore(const std::string &name, int score)
{
    // Add a new high score entry and maintain the list in descending order
    ScoreEntry newEntry = {name, score};
    scores.push_back(newEntry);

    // Sort the scores in descending order
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry &a, const ScoreEntry &b)
              { return a.score > b.score; });

    // Ensure we keep only the top N high scores (adjust N as needed)
    const int maxScores = 10;
    if (scores.size() > maxScores)
    {
        scores.pop_back();
    }

    // Save the updated high scores
    saveToFile("highscores.txt");
}

void HighScore::displayHighScores(sf::RenderWindow &window)
{
    sf::Font font;
    if (!font.loadFromFile("resources/INVASION2000.ttf"))
    {
        std::cerr << "Failed to load font file" << std::endl;
        return;
    }

    // Display the high scores on the window
    sf::Text highScoresText("High Scores:", font, 30);
    highScoresText.setFillColor(sf::Color::White);
    highScoresText.setPosition(50.0f, 50.0f);
    window.draw(highScoresText);

    float yOffset = 100.0f; // Vertical spacing between high scores

    for (const auto &entry : scores)
    {

        sf::Text scoreText(entry.name + ": " + std::to_string(entry.score), font, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(50.0f, yOffset);
        yOffset += 30.0f; // Increase vertical spacing
        window.draw(scoreText);
    }
}

void HighScore::clearHighScoresFile(const std::string &filename)
{
    std::ofstream file(filename, std::ios::trunc);
    file.close();
}

// Implementation of the getHighScores function
std::vector<HighScore::ScoreEntry> HighScore::getHighScores() const
{
    return scores;
}

