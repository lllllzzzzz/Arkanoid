#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include <SFML\Audio.hpp>

#include "gameState.hpp"
#include "..\objects\ball.hpp"
#include "..\objects\paddle.hpp"
#include "..\objects\brick.hpp"

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
    void generateNewBrickGrid(const int numBricksX, const int numBricksY);
    template <class T1, class T2>
    bool isIntersecting(T1& mA, T2& mB);
    void testCollision(Paddle& mPaddle, Ball& mBall);
    void testCollision(Brick& mBrick, Ball& mBall);

    static PlayState* Instance() {
        return &m_PlayState;
    }

    sf::SoundBuffer brickCollisionBuffer;
    sf::Sound brickCollisionSound;
    sf::SoundBuffer paddleCollisionBuffer;
    sf::Sound paddleCollisionSound;
    //Music backgroundMusic;

private:
    static PlayState m_PlayState;

    const int initialBallX{770 - 64};
    const int initialBallY{500 - 64};
    const float ballRadius{8.0f};
    const int initialPaddleX{770 / 2};
    const int initialPaddleY{500 - 30};
    const int paddleWidth{85};
    const int paddleHeight{10};

    const int numLivesDefault{2};

    Ball ball{initialBallX, initialBallY, ballRadius, sf::Color(255, 255, 255)};
    Paddle paddle{initialPaddleX, initialPaddleY, paddleWidth, paddleHeight, sf::Color(255, 255, 255)};
    std::vector<Brick> bricks;
    int numLives;
};

#endif // PLAY_STATE_HPP
