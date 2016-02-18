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

    void displayHud(const int playerScore, const int highScore, const int numLives, const int level);
    void displayPlayerScore(const int playerScore);
    void displayHighScore(const int highScore);
    void displayLevel(const int level);
    void displayNumLives(const int numLives);

    int getScoreBackgroundHeight() const noexcept { return scoreBackgroundHeight; }

private:
    GameEngine *m_engine;

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
