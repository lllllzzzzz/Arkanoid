#include "hud.hpp"

const sf::Color Hud::scoreBackgroundColour = sf::Color::Black;
const int Hud::scoreBackgroundWidth = 480;
const int Hud::scoreBackgroundHeight = 30;

Hud::Hud()
{
    if (!font.loadFromFile("data\\font\\PressStart2P.ttf")) {
    }
    if (!textureLives.loadFromFile("data\\graphics\\bat.png")) {
    }

    text.setFont(font);
    text.setCharacterSize(14);
    //text.setStyle(sf::Text::Bold);

    lives.setSize({40, 10});
    lives.setTexture(&textureLives);
    lives.setOrigin(50 / 2, 20 / 2);

    scoreBackground.setSize({scoreBackgroundWidth, scoreBackgroundHeight});
    scoreBackground.setFillColor(sf::Color::Black);
    scoreBackground.setOrigin(0, 0);
}

Hud::~Hud()
{

}

void Hud::displayHud(sf::RenderWindow *window, int playerScore, int highScore, int numLives, int level, bool playerIsPlaying)
{
    window->draw(scoreBackground);

    if (playerIsPlaying) {
        displayPlayerScore(window, playerScore);
        displayHighScore(window, std::max(playerScore, highScore));
    } else {
        text.setColor(sf::Color::White);
        text.setString("Press W to play");
        text.setPosition(15, 8);
        window->draw(text);
    }
    
    displayLevel(window, level);
    displayNumLives(window, numLives);
}

void Hud::displayPlayerScore(sf::RenderWindow *window, int playerScore)
{
    text.setColor(sf::Color::Green);
    text.setString(std::to_string(playerScore));
    text.setPosition(15, 8);
    window->draw(text);
}

void Hud::displayHighScore(sf::RenderWindow *window, int highScore)
{
    text.setColor(sf::Color::Red);
    text.setString(std::to_string(highScore));
    text.setPosition(150, 8);
    window->draw(text);
}

void Hud::displayLevel(sf::RenderWindow *window, int level)
{
    text.setColor(sf::Color::White);
    text.setString("Level " + std::to_string(level));
    text.setPosition(350, 8);
    window->draw(text);
}

void Hud::displayNumLives(sf::RenderWindow *window, int numLives)
{
    for (int i{0}; i < numLives; i++) {
        lives.setPosition((i * 43) + 50, 620 - 10);
        window->draw(lives);
    }
}
