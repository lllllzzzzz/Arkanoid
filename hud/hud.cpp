#include "hud.hpp"

#include <windows.h>

const int Hud::SCORE_BACKGROUND_WIDTH = 480;
const int Hud::SCORE_BACKGROUND_HEIGHT = 30;
const int Hud::TEXT_SIZE = 12;
const int Hud::LIVES_SPACING_X = 45;
const int Hud::LIVES_OFFSET_X = 50;
const int Hud::LIVES_ELEVATION = 10;
const sf::Vector2f Hud::PLAYER_SCORE_POSITION = {15, 9};
const sf::Vector2f Hud::HIGH_SCORE_POSITION = {100, 9};
const sf::Vector2f Hud::LEVEL_POSITION = {110, 9};
const sf::Vector2f Hud::PAUSED_POSITION = {250, 9};
const sf::Vector2f Hud::LIVES_SIZE = {40, 10};
const sf::Color Hud::SCORE_BACKGROUND_COLOUR = sf::Color::Black;
const sf::Color Hud::PLAYER_SCORE_COLOUR = sf::Color::White;
const sf::Color Hud::HIGH_SCORE_COLOUR = sf::Color::Red;
const sf::Color Hud::LEVEL_COLOUR = sf::Color::White;

Hud::Hud()
{

}

Hud::~Hud()
{

}

void Hud::Init(GameEngine *game)
{
    m_engine = game;

    text.setFont(m_engine->resourceMan.GetFont("PressStart2P.ttf"));
    text.setCharacterSize(TEXT_SIZE);

    lives.setSize(LIVES_SIZE);
    lives.setTexture(&m_engine->resourceMan.GetTexture("paddle.png"));
    lives.setOrigin(LIVES_SIZE.x / 2, LIVES_SIZE.y / 2);

    scoreBackground.setSize({m_engine->getWindowSize().x, SCORE_BACKGROUND_HEIGHT});
    scoreBackground.setFillColor(SCORE_BACKGROUND_COLOUR);
    scoreBackground.setOrigin(0, 0);
}

void Hud::displayHud(const int playerScore, const int highScore, const int numLives, const int level, const bool gameIsRunning)
{
    m_engine->getWindow().draw(scoreBackground);

    if (/*playerIsPlaying*/1) {
        displayPlayerScore(playerScore);
        displayHighScore(std::max(playerScore, highScore));
        displayLevel(level);
        displayNumLives(numLives);
    } else {
        text.setColor(sf::Color::White);
        text.setString("Press W to play");
        text.setPosition(15, 8);
        m_engine->getWindow().draw(text);
    }

    if (!gameIsRunning) {
        displayPaused();
    }
}

void Hud::displayPlayerScore(const int playerScore)
{
    text.setColor(PLAYER_SCORE_COLOUR);
    text.setString(std::to_string(playerScore));
    text.setPosition(PLAYER_SCORE_POSITION);
    m_engine->getWindow().draw(text);
}

void Hud::displayHighScore(const int highScore)
{
    text.setColor(HIGH_SCORE_COLOUR);
    text.setString(std::to_string(highScore));
    text.setPosition(HIGH_SCORE_POSITION);
    m_engine->getWindow().draw(text);
}

void Hud::displayLevel(const int level)
{
    text.setColor(LEVEL_COLOUR);
    text.setString("Level " + std::to_string(level));
    text.setPosition(m_engine->getWindowSize().x - LEVEL_POSITION.x, LEVEL_POSITION.y);
    m_engine->getWindow().draw(text);
}

void Hud::displayNumLives(const int numLives)
{
    for (int i{0}; i < numLives; i++) {
        lives.setPosition((i * LIVES_SPACING_X) + LIVES_OFFSET_X, m_engine->getWindowSize().y - LIVES_ELEVATION);
        m_engine->getWindow().draw(lives);
    }
}

void Hud::displayPaused()
{
    text.setColor(sf::Color::White);
    text.setString("Paused");
    text.setPosition(m_engine->getWindowSize().x - PAUSED_POSITION.x, PAUSED_POSITION.y);
    m_engine->getWindow().draw(text);
}
