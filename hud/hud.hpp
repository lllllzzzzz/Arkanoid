#ifndef HUD_HPP
#define HUD_HPP

#include <SFML\Graphics.hpp>
#include "..\stateman\gameEngine.hpp"
#include "..\states\gameState.hpp"

//#include "..\stateman\gameEngine.hpp"

class Hud
{
public:
    Hud();
    ~Hud();

    void Init(GameEngine *game);

    void displayHud(GameEngine *game, int playerScore, int highScore, int numLives, int level, bool playerIsPlaying);
    void displayPlayerScore(GameEngine *game, int playerScore);
    void displayHighScore(GameEngine *game, int highScore);
    void displayLevel(GameEngine *game, int level);
    void displayNumLives(GameEngine *game, int numLives);

    int getScoreBackgroundHeight() const noexcept { return scoreBackgroundHeight; }

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
