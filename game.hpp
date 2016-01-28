#ifndef GAME_HPP
#define GAME_HPP

#include "ball.hpp"
#include "paddle.hpp"
#include "brick.hpp"

#define windowWidth    750
#define windowHeight   450
#define initialBallX   (windowWidth - 64)
#define initialBallY   (windowHeight / 2)
#define ballRadius     8.0f
#define initialPaddleX (windowWidth / 2)
#define initialPaddleY (windowHeight - 25)
#define paddleWidth    80
#define paddleHeight   15
#define numLivesDefault 2

class Game
{
public:
    Game(int frameRate, int maxLives);
    ~Game();

    void runGame();
    void newGame();
    template <class T1, class T2>
    bool isIntersecting(T1& mA, T2& mB);
    void testCollision(Paddle& mPaddle, Ball& mBall);
    void testCollision(Brick& mBrick, Ball& mBall);

private:
    RenderWindow window{{750, 450}, "Arkanoid"};
    Ball ball{initialBallX, initialBallY, ballRadius};
    Paddle paddle{initialPaddleX, initialPaddleY, paddleWidth, paddleHeight};
    std::vector<Brick> bricks;
    int numLives;
};

#endif // GAME_HPP
