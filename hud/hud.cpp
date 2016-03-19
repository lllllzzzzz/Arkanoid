#include "hud.hpp"

const sf::Color Hud::SCORE_BG_COLOUR = sf::Color::Black;
const sf::Vector2f Hud::SCORE_BACKGROUND_SIZE = {480, 30};
const int Hud::NUM_LIVES_ELEVATION = 10;

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

    scoreBackground.setSize({m_engine->getWindowSize().x, SCORE_BACKGROUND_SIZE.y});
    scoreBackground.setFillColor(sf::Color::Black);
    scoreBackground.setOrigin(0, 0);
}

void Hud::DisplayHud(const int playerScore, const int highScore, const int numLives, const int level)
{
    m_engine->getWindow().draw(scoreBackground);

    if (/*playerIsPlaying*/1) {
        DisplayPlayerScore(playerScore);
        DisplayHighScore(std::max(playerScore, highScore));
        DisplayLevel(level);
        DisplayNumLives(numLives);
    } else {
        text.setColor(sf::Color::White);
        text.setString("Press W to play");
        text.setPosition(15, 8);
        m_engine->getWindow().draw(text);
    }
}

void Hud::DisplayPlayerScore(const int playerScore)
{
    text.setColor(sf::Color::Green);
    text.setString(std::to_string(playerScore));
    text.setPosition(15, 8);
    m_engine->getWindow().draw(text);
}

void Hud::DisplayHighScore(const int highScore)
{
    text.setColor(sf::Color::Red);
    text.setString(std::to_string(highScore));
    text.setPosition(m_engine->getWindowSize().x / 4, 8);
    m_engine->getWindow().draw(text);
}

void Hud::DisplayLevel(const int level)
{
    text.setColor(sf::Color::White);
    text.setString("Level " + std::to_string(level));
    text.setPosition(m_engine->getWindowSize().x - 110, 8);
    m_engine->getWindow().draw(text);
}

void Hud::DisplayNumLives(const int numLives)
{
    for (int i{0}; i < numLives; i++) {
        lives.setPosition((i * 43) + 50, m_engine->getWindowSize().y - NUM_LIVES_ELEVATION);
        m_engine->getWindow().draw(lives);
    }
}
