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

    void displayHud(const int playerScore, const int highScore, const int numLives, const int level, const bool gameIsRunning);
    void displayPlayerScore(const int playerScore);
    void displayHighScore(const int highScore);
    void displayLevel(const int level);
    void displayNumLives(const int numLives);
    void displayPaused();
    void displayWinLose(bool playerWon);

    int getScoreBackgroundHeight() const noexcept { return SCORE_BACKGROUND_HEIGHT; }

private:
    GameEngine *m_engine;

    sf::Text text;
    sf::Font font;
    sf::RectangleShape lives;
    sf::Texture textureLives;

    sf::RectangleShape scoreBackground;
    sf::RectangleShape winLoseBackground;
    sf::RectangleShape winLoseBackgroundBorder;

    static const int SCORE_BACKGROUND_WIDTH;
    static const int SCORE_BACKGROUND_HEIGHT;
    static const sf::Vector2f WIN_LOSE_BACKGROUND_SIZE;
    static const sf::Vector2f WIN_LOSE_BACKGROUND_BORDER_SIZE;
    static const int TEXT_SIZE;
    static const int LIVES_SPACING_X;
    static const int LIVES_OFFSET_X;
    static const int LIVES_ELEVATION;
    static const sf::Color SCORE_BACKGROUND_COLOUR;
    static const sf::Color WIN_LOSE_BACKGROUND_COLOUR;
    static const sf::Color WIN_LOSE_BACKGROUND_BORDER_COLOUR;
    static const sf::Color PLAYER_SCORE_COLOUR;
    static const sf::Color HIGH_SCORE_COLOUR;
    static const sf::Color LEVEL_COLOUR;
    static const sf::Vector2f PLAYER_SCORE_POSITION;
    static const sf::Vector2f HIGH_SCORE_POSITION;
    static const sf::Vector2f LEVEL_POSITION;
    static const sf::Vector2f PAUSED_POSITION;
    static const sf::Vector2f LIVES_SIZE;
};

#endif // HUD_HPP
