#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include <SFML\Audio.hpp>
//#include <chrono>

#include "gameState.hpp"
#include "..\hud\hud.hpp"
#include "..\objects\ball.hpp"
#include "..\objects\paddle.hpp"
#include "..\objects\brick.hpp"
#include "..\objects\brickGrid.hpp"
#include "..\objects\powerup.hpp"
//#include "..\objects\powerupExtraLife.hpp"

class PlayState : public GameState
{
public:
    PlayState();
    ~PlayState();

    void Pause();
    void Resume();

    void HandleEvents(GameEngine *game);
    void Update(GameEngine *game);
    void Draw(GameEngine *game);

    void loadResources();
    void newGame(GameEngine *game);
    void generateNewBrickGrid(const int numBricksX, const int numBricksY, const int width, const int height);
    void playBrickGridAnimation(GameEngine *game, const int numBricksX, const int numBricksY);
    bool allBricksVisible() const noexcept;
    void loadNextLevel();
    void setupBackground();

    bool isGameRunning() const noexcept { return m_isGameRunning; }
    void pauseGame() { m_isGameRunning = false; }
    void resumeGame() { m_isGameRunning = true; }

    inline bool isSoundEnabled() const noexcept { return m_isSoundEnabled; }
    inline void enableSound() { m_isSoundEnabled = true; }
    inline void disableSound() { m_isSoundEnabled = false; }
    inline bool isPlayerPlaying() { return m_isPlayerPlaying; }
    inline void playerStartPlaying() { m_isPlayerPlaying = true; }
    inline void playerStopPlaying() { m_isPlayerPlaying = false; }

    template <class T1, class T2>
    bool isIntersecting(T1& mA, T2& mB);
    void testCollision(Paddle& mPaddle, Ball& mBall);
    void testCollision(Brick& mBrick, Ball& mBall);
    void testCollision(Paddle& mPaddle, Powerup& mPowerup);

    static PlayState* Instance() {
        return &m_PlayState;
    }

    //std::chrono::high_resolution_clock::timePoint1;
    //std::chrono::high_resolution_clock::timePoint2;

    sf::SoundBuffer brickCollisionBuffer;
    sf::Sound brickCollisionSound;
    sf::SoundBuffer paddleCollisionBuffer;
    sf::Sound paddleCollisionSound;
    sf::SoundBuffer newGameBuffer;
    sf::Sound newGameSound;
    sf::SoundBuffer loseLifeBuffer;
    sf::Sound loseLifeSound;
    sf::SoundBuffer gainPowerupBuffer;
    sf::Sound gainPowerupSound;
    sf::Music backgroundMusic;

    sf::Texture textureBrick;
    sf::Texture texturePaddle;
    sf::Texture textureBall;
    sf::Texture texturePowerup;
    sf::Texture textureBorderSide;
    sf::Texture textureBorderTop;
    sf::Texture textureBorderCornerLeft;
    sf::Texture textureBorderCornerRight;
    sf::Texture textureShieldLeft;
    sf::Texture textureShieldRight;

private:
    static PlayState m_PlayState;

    bool m_isPlayerPlaying;
    bool m_isSoundEnabled;
    bool m_isGameRunning{true};

    int numLives;
    int level;
    int playerScore;
    int highScore;

    Hud hud;
    Paddle paddle;
    std::vector<Ball> balls;
    std::vector<Brick> bricks;
    std::vector<Powerup> powerups;
    //BrickGrid brickGrid;

    sf::RectangleShape borderTop;
    sf::RectangleShape borderLeft;
    sf::RectangleShape borderRight;
    sf::RectangleShape borderCornerLeft;
    sf::RectangleShape borderCornerRight;
    sf::RectangleShape shieldLeft;
    sf::RectangleShape shieldRight;
    sf::RectangleShape shadowTop;
    sf::RectangleShape shadowLeft;

    /*float lastFt;
    float currentSlice;
    static const float ftStep;
    static const float ftSlice;*/

    static const int initBallX;
    static const int initBallY;
    static const float ballRadius;
    static const sf::Color ballColour;

    static const int initPaddleX;
    static const int initPaddleY;
    static const int paddleWidth;
    static const int paddleHeight;
    static const sf::Color paddleColour;

    static const int countBricksX;
    static const int countBricksY;
    static const int brickWidth;
    static const int brickHeight;

    static const int powerupWidth;
    static const int powerupHeight;
    static const sf::Color powerupColour;

    static const int ballPoints;

    static const int numLivesDefault;

    static const int powerupProbability;
};

#endif // PLAY_STATE_HPP
