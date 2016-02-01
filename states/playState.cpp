#include "..\stateman\gameEngine.hpp"
#include "playState.hpp"
#include "menuState.hpp"

#include <iostream>
#include <chrono>

#ifdef _WIN32
//#include <windows.h>
#endif

//const float PlayState::ftStep = 1.f;
//const float PlayState::ftSlice = 1.f;

const int PlayState::initialPaddleX = 770 / 2;
const int PlayState::initialPaddleY = 600 - 30;
const int PlayState::paddleWidth = 85;
const int PlayState::paddleHeight = 10;

const int PlayState::initialBallX = 770 - 64;
const int PlayState::initialBallY = 500 - 64;
const float PlayState::ballRadius = 8.0f;

const int PlayState::countBricksX = 14;
const int PlayState::countBricksY = 10;
const int PlayState::brickWidth = 45;
const int PlayState::brickHeight = 15;

const int PlayState::numLivesDefault = 2;

PlayState PlayState::m_PlayState;

PlayState::PlayState():
    soundEnabled(true),
    highScore(0)
    //brickGrid(countBricksX, countBricksY, brickWidth, brickHeight)
    //lastFt{0.f},
    //currentSlice{0.f}
{
    if (!brickCollisionBuffer.loadFromFile("data\\sound\\brick_collision.wav")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: brick_collision.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    brickCollisionSound.setBuffer(brickCollisionBuffer);

    if (!paddleCollisionBuffer.loadFromFile("data\\sound\\paddle_collision.wav")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: paddle_collision.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    paddleCollisionSound.setBuffer(paddleCollisionBuffer);

    if (!loseLifeBuffer.loadFromFile("data\\sound\\lose_life.wav")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: lose_life.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    loseLifeSound.setBuffer(loseLifeBuffer);

    //backgroundMusic.openFromFile("data\\sound\\bgm.wav");
    //backgroundMusic.play();

    //std::cout << "PlayState constructor" << std::endl;

    font.loadFromFile("data\\font\\arial.ttf");
    str.setCharacterSize(18);
    //str.setStyle(sf::Text::Bold);
    str.setFont(font);

    newGame();

    /*lastFt = 0.f;
    currentSlice = 0.f;*/
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

    if (!balls.size()) {
        numLives--;
        //newGame();
    }

    if (!numLives) {
        newGame();
    }

    //timePoint1 = std::chrono::high_resolution_clock::now();

    // Poll window events, required so player can interact with game window
    sf::Event event;
    while (game->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game->window.close();
        } //else if (event.type == sf::Event::KeyPressed && event.keyCode == sf::Keyboard::Key::Escape) {
            //game->pushState(new MenuState());
        //}
    }

    // Exit/menu controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        //game->changeState(new MenuState());
        game->pushState(new MenuState());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        game->Quit();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (!balls.at(0).isLaunched) {
            balls.at(0).isLaunched = true;
            balls.at(0).setVelocity(0, -balls.at(0).ballVelocity);
        }
    }

    // Player has missed ball; lose a life and reset ball
    if (balls.size() > 0) {
        for (auto &ball : balls) {
            if (ball.y() > paddle.y()) {
                //std::cout << "Ball destroyed!" << std::endl;
                ball.isDestroyed = true;
            }
        }
    }

    // Perform collision detection between all balls and bricks
    // O(M*N) algorithm
    for (auto& ball : balls) {
        testCollision(paddle, ball);
        for (auto& brick : bricks) {
            testCollision(brick, ball);
        }
    }

    //auto ftSeconds(ft / 1000.f);
    //auto fps(1.f / ftSeconds);

    //game->window.setTitle(
    //    "FT: " + std::to_string(ft) + "\tFPS: " + to_string(fps));
}

void PlayState::Update(GameEngine *game)
{
    //std::cout << ftStep << ", " << ftSlice << "\n";
    //std::cout << "PlayState Update()" << std::endl;

    if (balls.at(0).isLaunched) {
        for (auto& ball : balls) {
            ball.update(game->getWindowWidth(), game->getWindowHeight());
        }
    } else {
        balls.at(0).setPos(paddle.x(), 545);
    }

    /*currentSlice += lastFt;
    for(; currentSlice >= ftSlice; currentSlice -= ftSlice) {*/
        //ball.update(/*ftStep, */game->getWindowWidth(), game->getWindowHeight());

        paddle.update(/*ftStep, */game->getWindowWidth(), game->getWindowHeight());

        // Lambda function to remove destroyed blocks from blocks vector
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

        balls.erase(
            remove_if(
                begin(balls),
                end(balls),
                [](const Ball& mBall) {
                    return mBall.isDestroyed;
                }
            ),
            end(balls)
        );

        if (balls.size() == 0) {
            newGame();
        }
    //}

    //if ((playerScore % (20 * balls.size()) == 0) && (playerScore > 0)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            //std::cout << "New ball" << std::endl;
            balls.emplace_back(paddle.x(), 300, ballRadius,
                sf::Color(255, 255, 255));
            balls.at(balls.size() - 1).isLaunched = true;
        //}
        //playerScore += 5;
    }

    //std::cout << balls.size() << std::endl;
}

void PlayState::Draw(GameEngine *game)
{
    //std::cout << "PlayState Draw()" << std::endl;

    if (!numLives) {
        newGame();
    }

    game->window.clear(sf::Color::Black);

    /*if (!allBricksVisible()) {
        bricks.at(std::rand() % bricks.size()).setVisibility(true);
    }*/

    if (numLives) {
        for (auto& ball : balls) {
            game->window.draw(ball.shape);
        }
    }

    game->window.draw(paddle.shape);
    for (auto& brick : bricks) {
        if (brick.isVisible()) {
            game->window.draw(brick.shape);
        }
    }

    sf::Text score;
    score.setCharacterSize(22);
    score.setFont(font);
    score.setColor(sf::Color::Green);
    //score.setStyle(sf::Text::Bold);

    score.setString(std::to_string(playerScore));
    score.setPosition(15, 10);
    game->window.draw(score);

    score.setColor(sf::Color::Red);
    score.setString(std::to_string(highScore));
    score.setPosition(110, 10);
    game->window.draw(score);

    score.setColor(sf::Color::Blue);
    score.setString(std::to_string(numLives));
    score.setPosition(game->getWindowWidth() - 50, 10);
    game->window.draw(score);

    game->window.display();

    /*timePoint2 = std::chrono::high_resolution_clock::now();
    auto elapsedTime(timePoint2 - timePoint1);
    float ft{std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
                     elapsedTime)
                     .count()};

    lastFt = ft;*/
}

void PlayState::newGame()
{
    //std::cout << "PlayState newGame()" << std::endl;

    loseLifeSound.play();
    highScore = (playerScore > highScore) ? playerScore : highScore;
    playerScore = 0;

    balls.clear();
    balls.emplace_back(initialBallX, initialBallY, ballRadius,
        sf::Color(255, 255, 255));
    balls.at(0).isLaunched = false;

    generateNewBrickGrid(countBricksX, countBricksY, brickWidth, brickHeight);

    paddle.setPos(initialPaddleX, initialPaddleY);
    balls.at(0).setPos(initialPaddleX, paddle.top() - 25);

    balls.at(0).setVelocity(0, 8.0f);

    numLives = numLivesDefault;
}

void PlayState::generateNewBrickGrid(const int numBricksX, const int numBricksY, const int width, const int height)
{
    for (int iX{0}; iX < numBricksX; iX++) {
        for (int iY{0}; iY < numBricksY; iY++) {
            // Generate random colour for brick
            //std::srand(std::time(0));

            int r = std::rand() % 256;
            int g = std::rand() % 256;
            int b = std::rand() % 256;
            sf::Color color(r, g, b);

            bricks.emplace_back(
                (iX + 1) * (width + 3) + 22, (iY + 4) * (height + 3),
                width, height, color);
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

    int velocityX = (mBall.x() < mPaddle.x()) ? -mBall.ballVelocity : mBall.ballVelocity;
    int velocityY = -mBall.ballVelocity;

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

    playerScore += 5;

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
        mBall.velocity.x = ballFromLeft ? -mBall.ballVelocity : mBall.ballVelocity;
    } else {
        mBall.velocity.y = ballFromTop ? -mBall.ballVelocity : mBall.ballVelocity;
    }
}

const bool PlayState::allBricksVisible()
{
    for (auto &brick : bricks) {
        if (!brick.isVisible()) {
            return false;
        }
    }

    return true;
}
