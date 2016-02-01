#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include <SFML\Audio.hpp>
//#include <chrono>

#include "gameState.hpp"
#include "..\objects\ball.hpp"
#include "..\objects\paddle.hpp"
#include "..\objects\brick.hpp"
#include "..\objects\brickGrid.hpp"

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

    void newGame();
    void generateNewBrickGrid(const int numBricksX, const int numBricksY, const int width, const int height);
    template <class T1, class T2>
    bool isIntersecting(T1& mA, T2& mB);
    void testCollision(Paddle& mPaddle, Ball& mBall);
    void testCollision(Brick& mBrick, Ball& mBall);

    const bool allBricksVisible();

    static PlayState* Instance() {
        return &m_PlayState;
    }

    sf::SoundBuffer brickCollisionBuffer;
    sf::Sound brickCollisionSound;
    sf::SoundBuffer paddleCollisionBuffer;
    sf::Sound paddleCollisionSound;
    sf::SoundBuffer loseLifeBuffer;
    sf::Sound loseLifeSound;
    //Music backgroundMusic;

    //std::chrono::high_resolution_clock::timePoint1;
    //std::chrono::high_resolution_clock::timePoint2;

private:
    static PlayState m_PlayState;

    static const int initialBallX;
    static const int initialBallY;
    static const float ballRadius;
    static const int initialPaddleX;
    static const int initialPaddleY;
    static const int paddleWidth;
    static const int paddleHeight;
    static const int numLivesDefault;
    static const int countBricksX;
    static const int countBricksY;
    static const int brickWidth;
    static const int brickHeight;

    //Ball ball{initialBallX, initialBallY, ballRadius, sf::Color(255, 255, 255)};
    std::vector<Ball> balls;
    Paddle paddle{initialPaddleX, initialPaddleY, paddleWidth, paddleHeight, sf::Color(255, 255, 255)};
    std::vector<Brick> bricks;
    //BrickGrid brickGrid;
    int numLives;

    /*float lastFt;
    float currentSlice;
    static const float ftStep;
    static const float ftSlice;*/

    sf::Text str;
    sf::Font font;

    int playerScore;
    int highScore;

    bool soundEnabled;
};

#endif // PLAY_STATE_HPP
