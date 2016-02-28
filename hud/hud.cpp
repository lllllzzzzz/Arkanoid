#include "hud.hpp"

#include <windows.h>

const sf::Color Hud::scoreBackgroundColour = sf::Color::Black;
const int Hud::scoreBackgroundWidth = 480;
const int Hud::scoreBackgroundHeight = 30;

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
    text.setCharacterSize(14);

    lives.setSize({40, 10});
    lives.setTexture(&m_engine->resourceMan.GetTexture("paddle.png"));
    lives.setOrigin(50 / 2, 20 / 2);

    scoreBackground.setSize({m_engine->getWindowSize().x, scoreBackgroundHeight});
    scoreBackground.setFillColor(sf::Color::Black);
    scoreBackground.setOrigin(0, 0);
}

void Hud::displayHud(const int playerScore, const int highScore, const int numLives, const int level)
{
    m_engine->getWindow().draw(scoreBackground);

    //if (playerIsPlaying) {
        displayPlayerScore(playerScore);
        displayHighScore(std::max(playerScore, highScore));
        displayLevel(level);
        displayNumLives(numLives);
    /*} else {
        text.setColor(sf::Color::White);
        text.setString("Press W to play");
        text.setPosition(15, 8);
        m_engine->getWindow().draw(text);
    }*/
}

void Hud::displayPlayerScore(const int playerScore)
{
    text.setColor(sf::Color::Green);
    text.setString(std::to_string(playerScore));
    text.setPosition(15, 8);
    m_engine->getWindow().draw(text);
}

void Hud::displayHighScore(const int highScore)
{
    text.setColor(sf::Color::Red);
    text.setString(std::to_string(highScore));
    text.setPosition(m_engine->getWindowSize().x / 4, 8);
    m_engine->getWindow().draw(text);
}

void Hud::displayLevel(const int level)
{
    text.setColor(sf::Color::White);
    text.setString("Level " + std::to_string(level));
    text.setPosition(m_engine->getWindowSize().x - 110, 8);
    m_engine->getWindow().draw(text);
}

void Hud::displayNumLives(const int numLives)
{
    for (int i{0}; i < numLives; i++) {
        lives.setPosition((i * 43) + 50, m_engine->getWindowSize().y - 10);
        m_engine->getWindow().draw(lives);
    }
}
