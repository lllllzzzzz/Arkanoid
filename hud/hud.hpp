#ifndef HUD_HPP
#define HUD_HPP

#include <SFML\Graphics.hpp>
#include "..\stateman\gameEngine.hpp"
#include "..\states\gameState.hpp"

class Hud
{
public:
    Hud();
    ~Hud();

    void Init(GameEngine *game);

    void DisplayHud(const int playerScore, const int highScore, const int numLives, const int level);
    void DisplayPlayerScore(const int playerScore);
    void DisplayHighScore(const int highScore);
    void DisplayLevel(const int level);
    void DisplayNumLives(const int numLives);

    sf::Vector2f GetScoreBackgroundSize() const noexcept { return SCORE_BACKGROUND_SIZE; }

private:
    GameEngine *m_engine;

    sf::Text text;
    sf::Font font;
    sf::RectangleShape lives;
    sf::Texture textureLives;

    sf::RectangleShape scoreBackground;

    static const sf::Color SCORE_BG_COLOUR;
    const sf::Vector2f SCORE_BACKGROUND_SIZE;
    static const int NUM_LIVES_ELEVATION;
};

#endif // HUD_HPP
