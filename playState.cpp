#include "..\stateman\gameEngine.hpp"
#include "playState.hpp"
#include "menuState.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

PlayState PlayState::m_PlayState;

PlayState::PlayState()
{
    if (!brickCollisionBuffer.loadFromFile("data\\sound\\brick_collision.wav")) {
        #ifdef _WIN32
        MessageBox(NULL, "Error loading file: brick_collision.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    brickCollisionSound.setBuffer(brickCollisionBuffer);

    if (!brickCollisionBuffer.loadFromFile("data\\sound\\paddle_collision.wav")) {
        #ifdef _WIN32
        MessageBox(NULL, "Error loading file: paddle_collision.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    paddleCollisionSound.setBuffer(paddleCollisionBuffer);

    //backgroundMusic.openFromFile("data\\sound\\bgm.wav");
    //backgroundMusic.play();

    //std::cout << "PlayState constructor" << std::endl;
    numLives = numLivesDefault;
    newGame();
}

PlayState::~PlayState()
{

}

void PlayState::Pause()
{

}

void PlayState::Resume()
{

}

void PlayState::HandleEvents(GameEngine *game)
{
    //std::cout << "PlayState HandleEvents()" << std::endl;
    //backgroundMusic.play();

    // Poll window events, required so player can interact with game window
    sf::Event event;
    while (game->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game->window.close();
        }
    }

    // Exit/menu controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        game->pushState(new MenuState());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        game->Quit();
    }

    if (!numLives) {
        newGame();
    }

    // Player has missed ball; lose a life and reset ball
    if (ball.top() >= paddle.y()) {
        numLives--;
        ball.setPos(ball.initialPosX, ball.initialPosY);
        ball.setVelocity(-ball.ballVelocity, -ball.ballVelocity);
    }

    // Perform collision detection
    testCollision(paddle, ball);
    for (auto& brick : bricks) {
        testCollision(brick, ball);
    }
}

void PlayState::Update(GameEngine *game)
{
    //std::cout << "PlayState Update()" << std::endl;

    ball.update(game->windowWidth, game->windowHeight);
    paddle.update();

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
}

void PlayState::Draw(GameEngine *game)
{
    //std::cout << "PlayState Draw()" << std::endl;

    game->window.clear(sf::Color::Black);

    game->window.draw(ball.shape);
    game->window.draw(paddle.shape);
    for (auto& brick : bricks) {
        game->window.draw(brick.shape);
    }

    game->window.display();
}

void PlayState::newGame()
{
    //std::cout << "PlayState newGame()" << std::endl;

    generateNewBrickGrid(countBricksX, countBricksY);

    ball.setPos(initialBallX, initialBallY);
    paddle.setPos(initialPaddleX, initialPaddleY);

    numLives = numLivesDefault;
}

void PlayState::generateNewBrickGrid(const int numBricksX, const int numBricksY)
{
    for (int iX{0}; iX < numBricksX; iX++) {
        for (int iY{0}; iY < numBricksY; iY++) {
            bricks.emplace_back((iX + 1) * (brickWidth + 3) + 22,
                (iY + 2) * (brickHeight + 3));
        }
    }
}

template <class T1, class T2>
bool PlayState::isIntersecting(T1& mA, T2& mB)
{
    //std::cout << "PlayState isIntersecting()" << std::endl;

    return mA.right() >= mB.left() && mA.left() <= mB.right()
        && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void PlayState::testCollision(Paddle& mPaddle, Ball& mBall)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (!isIntersecting(mPaddle, mBall)) {
        return;
    }

    paddleCollisionSound.play();

    int velocityX = (mBall.x() < mPaddle.x()) ? -ball.ballVelocity : ball.ballVelocity;
    int velocityY = -ball.ballVelocity;

    // Realistic paddle physics
    //int newX = abs(mPaddle.x() - mBall.x()) / 20;
    //velocityY = abs(velocityX - newX);
    //velocityX *= abs(mPaddle.x() - mBall.x()) / 20;

    mBall.setVelocity(velocityX, velocityY);
}

void PlayState::testCollision(Brick& mBrick, Ball& mBall)
{
    //std::cout << "PlayState testCollision() brick" << std::endl;

    if (!isIntersecting(mBrick, mBall)) {
        return;
    }

    mBrick.isDestroyed = true;
    brickCollisionSound.play();

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
        mBall.velocity.x = ballFromLeft ? -ball.ballVelocity : ball.ballVelocity;
    } else {
        mBall.velocity.y = ballFromTop ? -ball.ballVelocity : ball.ballVelocity;
    }
}
