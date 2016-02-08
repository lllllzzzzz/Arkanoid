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
    textureLives = game->resourceMan.GetTexture("paddle.png");
    font = game->resourceMan.GetFont("PressStart2P.ttf");

    text.setFont(font);
    text.setCharacterSize(14);
    //text.setStyle(sf::Text::Bold);

    lives.setSize({40, 10});
    lives.setTexture(&textureLives);
    lives.setOrigin(50 / 2, 20 / 2);

    scoreBackground.setSize({game->getWindowWidth(), scoreBackgroundHeight});
    scoreBackground.setFillColor(sf::Color::Black);
    scoreBackground.setOrigin(0, 0);
}

void Hud::displayHud(GameEngine *game, int playerScore, int highScore, int numLives, int level, bool playerIsPlaying)
{
    game->getWindow().draw(scoreBackground);

    if (playerIsPlaying) {
        displayPlayerScore(game, playerScore);
        displayHighScore(game, std::max(playerScore, highScore));
        displayLevel(game, level);
        displayNumLives(game, numLives);
    } else {
        text.setColor(sf::Color::White);
        text.setString("Press W to play");
        text.setPosition(15, 8);
        game->getWindow().draw(text);
    }
}

void Hud::displayPlayerScore(GameEngine *game, int playerScore)
{
    text.setColor(sf::Color::Green);
    text.setString(std::to_string(playerScore));
    text.setPosition(15, 8);
    game->getWindow().draw(text);
}

void Hud::displayHighScore(GameEngine *game, int highScore)
{
    text.setColor(sf::Color::Red);
    text.setString(std::to_string(highScore));
    text.setPosition(game->getWindowWidth() / 3, 8);
    game->getWindow().draw(text);
}

void Hud::displayLevel(GameEngine *game, int level)
{
    text.setColor(sf::Color::White);
    text.setString("Level " + std::to_string(level));
    text.setPosition(game->getWindowWidth() - 110, 8);
    game->getWindow().draw(text);
}

void Hud::displayNumLives(GameEngine *game, int numLives)
{
    for (int i{0}; i < numLives; i++) {
        lives.setPosition((i * 43) + 50, game->getWindowHeight() - 10);
        game->getWindow().draw(lives);
    }
}
