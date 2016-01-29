#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Audio.hpp>
#include <stack>

#include "gameState.hpp"
#include "ball.hpp"
#include "paddle.hpp"
#include "brick.hpp"

#define windowWidth    770
#define windowHeight   500
#define initialBallX   (windowWidth - 64)
#define initialBallY   (windowHeight / 2)
#define ballRadius     6.0f
#define initialPaddleX (windowWidth / 2)
#define initialPaddleY (windowHeight - 30)
#define paddleWidth    85
#define paddleHeight   10
#define numLivesDefault 2

class Game
{
public:
    Game(int frameRate, int maxLives);
    ~Game();

    void pushState(GameState* state);
    void popState();
    GameState* peekState();
    void changeState(GameState* state);

    void runGame();
    void newGame();
    template <class T1, class T2>
    bool isIntersecting(T1& mA, T2& mB);
    void testCollision(Paddle& mPaddle, Ball& mBall);
    void testCollision(Brick& mBrick, Ball& mBall);

    std::stack<GameState*> states;

    SoundBuffer brickCollisionBuffer;
    Sound brickCollisionSound;

    SoundBuffer paddleCollisionBuffer;
    Sound paddleCollisionSound;

    //Music backgroundMusic;

private:
    RenderWindow window{{windowWidth, windowHeight}, "Arkanoid"};
    Ball ball{initialBallX, initialBallY, ballRadius};
    Paddle paddle{initialPaddleX, initialPaddleY, paddleWidth, paddleHeight};
    std::vector<Brick> bricks;
    int numLives;
};

#endif // GAME_HPP
