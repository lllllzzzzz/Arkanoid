#include "game.hpp"
#include <iostream>

Game::Game(int frameRate, int maxLives)
{
    brickCollisionBuffer.loadFromFile("data\\sound\\brick_collision.wav");
    brickCollisionSound.setBuffer(brickCollisionBuffer);

    paddleCollisionBuffer.loadFromFile("data\\sound\\paddle_collision.wav");
    paddleCollisionSound.setBuffer(paddleCollisionBuffer);

    //backgroundMusic.openFromFile("data\\sound\\bgm.wav");
    //backgroundMusic.play();

    window.setFramerateLimit(frameRate);
    numLives = maxLives;
}

Game::~Game()
{

}

void Game::runGame()
{
    while (!Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
        // Poll window events, required so player can interact with game window
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear(Color::Black);

        //backgroundMusic.play();

        if (!numLives) {
            newGame();
        }

        // Player has missed ball; lose a life and reset ball
        if (ball.top() >= initialPaddleY) {
            numLives--;
            ball.setPos(initialBallX, initialBallY);
            ball.setVelocity(-ballVelocity, -ballVelocity);
        }

        ball.update();   // Move ball and check bounds
        paddle.update(); // Accept player input to move paddle

        // Perform collision detection
        testCollision(paddle, ball);
        for (auto& brick : bricks) {
            testCollision(brick, ball);
        }

        // Lambda function to remove destroyed blocks from block vector
        bricks.erase(
            remove_if(
                begin(bricks),
                end(bricks),
                [](const Brick& mBrick) {
                    return mBrick.isDestroyed;
                }
            ),
            end(bricks)
        );

        // Draw game objects
        window.draw(ball.shape);
        window.draw(paddle.shape);
        for (auto& brick : bricks) {
            window.draw(brick.shape);
        }

        window.display();
    }
}

void Game::newGame()
{
    // Generate bricks vector
    for (int iX{0}; iX < countBricksX; iX++) {
        for (int iY{0}; iY < countBricksY; iY++) {
            bricks.emplace_back((iX + 1) * (brickWidth + 3) + 22,
                (iY + 2) * (brickHeight + 3));
        }
    }

    ball.setPos(initialBallX, initialBallY);
    paddle.setPos(initialPaddleX, initialPaddleY);

    numLives = numLivesDefault;
}

template <class T1, class T2>
bool Game::isIntersecting(T1& mA, T2& mB)
{
    return mA.right() >= mB.left() && mA.left() <= mB.right()
        && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void Game::testCollision(Paddle& mPaddle, Ball& mBall)
{
    if (!isIntersecting(mPaddle, mBall)) {
        return;
    }

    paddleCollisionSound.play();

    int velocityX = (mBall.x() < mPaddle.x()) ? -ballVelocity : ballVelocity;
    int velocityY = -ballVelocity;

    // Realistic paddle physics
    //velocityX *= abs(mPaddle.x() - mBall.x()) / 20;

    mBall.setVelocity(velocityX, velocityY);
}

void Game::testCollision(Brick& mBrick, Ball& mBall)
{
    if (!isIntersecting(mBrick, mBall)) {
        return;
    }

    brickCollisionSound.play();

    mBrick.isDestroyed = true;

    // Calculate intersections of ball/brick
    float overlapLeft{mBall.right() - mBrick.left()};
    float overlapRight{mBrick.right() - mBall.left()};
    float overlapTop{mBall.bottom() - mBrick.top()};
    float overlapBottom{mBrick.bottom() - mBall.top()};

    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
    float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

    if (abs(minOverlapX) < abs(minOverlapY)) {
        mBall.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
    } else {
        mBall.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;
    }
}
