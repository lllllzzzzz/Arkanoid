#ifndef HUD_HPP
#define HUD_HPP

#include <SFML\Graphics.hpp>
#include "..\states\gameState.hpp"

class Hud
{
public:
    Hud();
    ~Hud();

    void displayHud(sf::RenderWindow *window, int playerScore, int highScore, int numLives, int level, bool playerIsPlaying);
    void displayPlayerScore(sf::RenderWindow *window, int playerScore);
    void displayHighScore(sf::RenderWindow *window, int highScore);
    void displayLevel(sf::RenderWindow *window, int level);
    void displayNumLives(sf::RenderWindow *window, int numLives);

private:
    sf::Text text;
    sf::Font font;
    sf::RectangleShape lives;
    sf::Texture textureLives;

    sf::RectangleShape scoreBackground;

    static const sf::Color scoreBackgroundColour;
    static const int scoreBackgroundWidth;
    static const int scoreBackgroundHeight;
};

#endif // HUD_HPP
