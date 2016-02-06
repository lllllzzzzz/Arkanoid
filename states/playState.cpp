#include "..\stateman\gameEngine.hpp"
#include "playState.hpp"
//#include "menuState.hpp"

#include <iostream>
//#include <chrono>

#ifdef _WIN32
//#include <windows.h>
#endif

//const float PlayState::ftStep = 1.f;
//const float PlayState::ftSlice = 1.f;

const int PlayState::initPaddleX = 480 / 2;
const int PlayState::initPaddleY = 620 - 70;

const int PlayState::initBallX = 620 - 64;
const int PlayState::initBallY = 580 - 64;
const float PlayState::ballRadius = 7.0f;
const sf::Color PlayState::ballColour = sf::Color(255, 255, 255);

const int PlayState::countBricksX = 11;
const int PlayState::countBricksY = 6;
const int PlayState::brickWidth = 40;
const int PlayState::brickHeight = 20;

const int PlayState::powerupWidth = 40;
const int PlayState::powerupHeight = 20;
const sf::Color PlayState::powerupColour = sf::Color(255, 255, 255);

const int PlayState::ballPoints = 5;

const int PlayState::numLivesDefault = 2;

const int PlayState::powerupProbability = 25;

PlayState PlayState::m_PlayState;

//using namespace Arkanoid;

PlayState::PlayState() :
    m_isSoundEnabled(true),
    highScore(0),
    numLives(0),
    level(1),
    m_isPlayerPlaying(true),
    paddle(/*initPaddleX, initPaddleY, paddleWidth, paddleHeight, paddleColour*/)
    //brickGrid(countBricksX, countBricksY, brickWidth, brickHeight)
    //lastFt{0.f},
    //currentSlice{0.f}
{
    std::cout << "PlayState constructor" << std::endl;

    //newGame();

    //loadResources();
    //setupBackground();

    /*lastFt = 0.f;
    currentSlice = 0.f;*/

    std::srand(time(0));
}

PlayState::~PlayState()
{

}

void PlayState::Init(GameEngine *game)
{
    LoadResources(game);
    LoadObjects();
    hud.Init(game);
    paddle.Init(game);
}

void PlayState::Cleanup(GameEngine *game)
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

    //timePoint1 = std::chrono::high_resolution_clock::now();

    // Poll window events, required so player can interact with game window
    sf::Event event;
    while (game->getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game->getWindow().close();
        } else if (event.type == sf::Event::LostFocus) {
            pauseGame();
        } else if (event.type == sf::Event::GainedFocus) {
            resumeGame();
        }
    }

    if (!isGameRunning()) {
        return;
    }

    // Accept player input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        game->Quit();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        game->pushState(/*PausedState::Instance()*/new PausedState());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        if (balls.size() == 1) {
            if (!balls.front().isLaunched()) {
                balls.front().launch();
                balls.front().setVelocity(0, -balls.front().getVelocity());
            }
        }
    }

    if (!numLives) {
        newGame(game);
    }

    if (bricks.empty()) {
        loadNextLevel();
    }

    // Player has missed ball; lose a life and reset ball
    if (!balls.empty()) {
        for (auto &ball : balls) {
            if (ball.y() > paddle.y()) {
                //std::cout << "Ball destroyed!" << std::endl;
                ball.destroy();
            }
        }
    }

    if (balls.empty()) {
        numLives--;
        balls.emplace_back(paddle.x(), paddle.y() - paddle.getHeight() / 2, ballRadius, ballColour, textureBall);
        //balls.front().launch();
        if (isPlayerPlaying()) {
            if (isSoundEnabled()) {
                loseLifeSound.play();
            }
        }
    }

    if (!numLives) {
        newGame(game);
    }

    // Perform collision detection between all balls and bricks
    // O(N*M) algorithm
    for (auto& ball : balls) {
        testCollision(paddle, ball);
        for (auto& brick : bricks) {
            testCollision(brick, ball);
        }
    }

    // Perform collision detection between paddle and all powerups
    for (auto& powerup : powerups) {
        testCollision(paddle, powerup);
    }

    //auto ftSeconds(ft / 1000.f);
    //auto fps(1.f / ftSeconds);

    //game->getWindow().setTitle(
    //    "FT: " + std::to_string(ft) + "\tFPS: " + to_string(fps));
}

void PlayState::Update(GameEngine *game)
{
    //std::cout << ftStep << ", " << ftSlice << "\n";
    //std::cout << "PlayState Update()" << std::endl;

    if (!isGameRunning()) {
        return;
    }

    if (!balls.empty()) {
        if (balls.front().isLaunched()) {
            for (auto& ball : balls) {
                ball.update(game->getWindowWidth(), game->getWindowHeight());
            }
        } else {
            balls.front().setPos(paddle.x() - balls.front().getRadius() / 2, paddle.y() - ((balls.front().getRadius() * 2) + 3));
        }
    }

    for (auto& powerup : powerups) {
        powerup.update(game->getWindowWidth(), game->getWindowHeight());
        //std::cout << "Updating powerup" << std::endl;
    }

    /*currentSlice += lastFt;
    for(; currentSlice >= ftSlice; currentSlice -= ftSlice) {*/
        //ball.update(/*ftStep, */game->getWindowWidth(), game->getWindowHeight());

        paddle.update(/*ftStep, */game->getWindowWidth(), game->getWindowHeight());

        // Remove destroyed blocks from blocks vector
        bricks.erase(
            remove_if(begin(bricks), end(bricks),
                [](const Brick& mBrick) { return mBrick.isDestroyed(); }),
            end(bricks)
        );

        // Remove destroyed balls from balls vector
        balls.erase(
            remove_if(begin(balls), end(balls),
                [](const Ball& mBall) {return mBall.isDestroyed(); }),
            end(balls)
        );

        // Remove destroyed powerups from powerups vector
        powerups.erase(
            remove_if(begin(powerups), end(powerups),
                [](const Powerup& mPowerup) { return mPowerup.isDestroyed(); }),
            end(powerups)
        );
    //}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        balls.back().launch();
        balls.emplace_back(paddle.x() - balls.back().getRadius() / 2, paddle.y() - 20, ballRadius, ballColour, textureBall);
        balls.back().launch();
        balls.back().setVelocity(0, -32);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        bricks.clear();
    }
}

void PlayState::Draw(GameEngine *game)
{
    //std::cout << "PlayState Draw()" << std::endl;

    if (!isGameRunning()) {
        return;
    }

    game->getWindow().clear(sf::Color(0, 0, 98));

    // Draw background
    game->getWindow().draw(shadowTop);
    game->getWindow().draw(shadowLeft);
    game->getWindow().draw(borderTop);
    game->getWindow().draw(borderLeft);
    game->getWindow().draw(borderCornerLeft);
    game->getWindow().draw(borderCornerRight);
    game->getWindow().draw(shieldLeft);
    //game->getWindow().draw(shieldRight);
    /*game->getWindow().draw(borderRight);*/

    // Draw bricks
    for (auto& brick : bricks) {
        if (brick.isVisible()) {
            game->getWindow().draw(brick.shape);
            game->getWindow().draw(brick.shadow);
        }
    }

    // Draw balls
    for (auto& ball : balls) {
        if (!ball.isDestroyed()) {
            game->getWindow().draw(ball.shadow);
            game->getWindow().draw(ball.shape);
        }
    }

    // Draw paddle
    game->getWindow().draw(paddle.shadow);
    game->getWindow().draw(paddle.shape);

    // Draw background
    game->getWindow().draw(borderRight);
    game->getWindow().draw(shieldRight);

    // Draw powerups
    for (auto& powerup : powerups) {
        game->getWindow().draw(powerup.shape);
    }

    // Draw HUD
    hud.displayHud(&game->getWindow(), playerScore, highScore, numLives, level, isPlayerPlaying());

    game->getWindow().display();

    /*timePoint2 = std::chrono::high_resolution_clock::now();
    auto elapsedTime(timePoint2 - timePoint1);
    float ft{std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
                     elapsedTime)
                     .count()};

    lastFt = ft;*/
}

void PlayState::newGame(GameEngine *game)
{
    //std::cout << "PlayState newGame()" << std::endl;

    level = 1;
    highScore = std::max(playerScore, highScore);
    playerScore = 0;
    numLives = numLivesDefault;

    currentLevel.clear();
    currentLevel = level1;
    generateNewBrickGrid(countBricksX, countBricksY * level, brickWidth, brickHeight);
    //playBrickGridAnimation(game, countBricksX, countBricksY);

    paddle.setPos(initPaddleX, initPaddleY);

    powerups.clear();

    balls.clear();
    balls.emplace_back(initBallX, initBallY, ballRadius, ballColour, textureBall);
    balls.front().setPos(initPaddleX, paddle.top() - (balls.front().getRadius() * 2) + 1);

    if (isPlayerPlaying()) {
        newGameSound.play();
        if (isSoundEnabled()) {
            backgroundMusic.play();
        }
    }
}

void PlayState::generateNewBrickGrid(const int numBricksX, const int numBricksY, const int width, const int height)
{
    bricks.clear();

    for (int i{0}; i < currentLevel.size(); i++) {
        int r = std::rand() % 256;
        int g = std::rand() % 256;
        int b = std::rand() % 256;
        sf::Color colour(r, g, b);

        bricks.emplace_back(
            (currentLevel.at(i).x + 1) * 40, (currentLevel.at(i).y * brickHeight)+ 150,
            width, height, currentLevel.at(i).colour, textureBrick);
    }
}

void PlayState::loadNextLevel()
{
    level++;

    paddle.setPos(initPaddleX, initPaddleY);
    powerups.clear();
    balls.clear();
    balls.emplace_back(initBallX, initBallY, ballRadius, ballColour, textureBall);
    balls.front().setPos(initPaddleX, paddle.top() - (balls.front().getRadius() * 2) + 1);

    if (isPlayerPlaying()) {
        newGameSound.play();
        if (isSoundEnabled()) {
            backgroundMusic.play();
        }
    }

    currentLevel.clear();

    switch (level) {
        case 1:
            currentLevel = level1;
            break;
        case 2:
            currentLevel = level2;
            break;
        default:
            level = 1;
            currentLevel = level1;
            break;
    }

    generateNewBrickGrid(countBricksX, countBricksY + (level - 1), brickWidth, brickHeight);
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

    float intersectX{abs(mPaddle.x() - mBall.x()) - 4};
    float relativeIntersectX{/*paddle.x() - intersectX*/intersectX};
    float normalizedRelativeIntersectionX{(relativeIntersectX / (paddle.getWidth() / 2))};
    float bounceAngle{normalizedRelativeIntersectionX * (3 * 3.14) / 12};

    std::cout << intersectX << std::endl;
    std::cout << relativeIntersectX << std::endl;
    std::cout << normalizedRelativeIntersectionX << std::endl;
    std::cout << bounceAngle << std::endl << std::endl;

    int ballVx{mBall.getVelocity() * sin(bounceAngle)};
    int ballVy{mBall.getVelocity() * cos(bounceAngle)};

    mBall.setVelocityX(ballVx);
    mBall.setVelocityY(ballVy);

    ballVx = (mBall.x() < mPaddle.x()) ? -ballVx : ballVx;

    std::cout << ballVx << ", " << ballVy << std::endl;

    mBall.setVelocity(ballVx, -ballVy);

    if (isSoundEnabled()) {
        paddleCollisionSound.play();
    }
}

void PlayState::testCollision(Brick& mBrick, Ball& mBall)
{
    //std::cout << "PlayState testCollision() brick" << std::endl;

    if (!isIntersecting(mBrick, mBall)) {
        return;
    }

    mBrick.destroy();

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
        mBall.setVelocityX(-mBall.getVelocityX());
        mBall.setVelocityY(mBrick.y() < mBall.y() ? abs(mBall.getVelocityY()) : -abs(mBall.getVelocityY()));
    } else {
        mBall.setVelocityX(mBall.x() < mBrick.x() ? -mBall.getVelocityX() : mBall.getVelocityX());
        mBall.setVelocityY(ballFromTop ? -abs(mBall.getVelocityY()) : abs(mBall.getVelocityY()));
    }

    if (!((std::rand() % 100) % powerupProbability)) {
        powerups.emplace_back(
            mBrick.x(), mBrick.y(),
            powerupWidth, powerupHeight, /*speed*/ 2, powerupColour, texturePowerup);
    }

    if (isSoundEnabled()) {
        brickCollisionSound.play();
    }
}

void PlayState::testCollision(Paddle& mPaddle, Powerup& mPowerup)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (!isIntersecting(mPaddle, mPowerup)) {
        return;
    }

    mPowerup.destroy();

    int n = std::rand() % 5;
    switch (n) {
        case 0:
            numLives++;
            break;
        case 1:
            paddle.setSize(paddle.getWidth() + 15, paddle.getHeight());
            paddle.setPos(paddle.x() + 0, paddle.y());
            paddle.shape.setOrigin(paddle.getWidth() / 2, paddle.getHeight() / 2);
            balls.front().setPos(balls.front().x() - 0, balls.front().y());
            break;
        case 2:
            for (auto& ball : balls) {
                ball.setSpeed(ball.getSpeed() - 2);
            }
            break;
        case 3:
            balls.back().launch();
            balls.emplace_back(paddle.x() - balls.back().radius() / 2, paddle.y() - 20, ballRadius, ballColour, textureBall);
            balls.back().launch();
            balls.back().setVelocity(0, -balls.back().getSpeed());
            break;
        case 4:
            paddle.setVelocity(paddle.getVelocity() + 2);
            break;
        default:
            break;
    }

    if (isSoundEnabled()) {
        gainPowerupSound.play();
    }
}

void PlayState::playBrickGridAnimation(GameEngine *game, const int numBricksX, const int numBricksY)
{
    for (int i{0}; i < numBricksY; i += 1) {
        for (int j{i}; j < numBricksX; j++) {
            bricks.at(j).setVisibility(true);
            //std::cout << j << ", " << i << std::endl;
        }

        game->getWindow().clear(sf::Color::Black);

        game->getWindow().draw(paddle.shape);

        for (auto& brick : bricks) {
            if (brick.isVisible()) {
                game->getWindow().draw(brick.shape);
                //game->getWindow().draw(brick.shadow);
                return;
            }
        }

        game->getWindow().display();
    }
}

bool PlayState::allBricksVisible() const noexcept
{
    for (auto &brick : bricks) {
        if (!brick.isVisible()) {
            return false;
        }
    }

    return true;
}

void PlayState::LoadResources(GameEngine *game)
{
    // Load textures
    textureBrick = game->resourceMan.GetTexture("brick.png");
    textureBall = game->resourceMan.GetTexture("ball.png");
    texturePowerup = game->resourceMan.GetTexture("powerup_extra_life.png");
    textureBorderSide = game->resourceMan.GetTexture("border_side.png");
    textureBorderTop = game->resourceMan.GetTexture("border_top.png");
    textureBorderCornerLeft = game->resourceMan.GetTexture("border_corner_left.png");
    textureBorderCornerRight = game->resourceMan.GetTexture("border_corner_right.png");
    textureShieldLeft = game->resourceMan.GetTexture("shield_left.png");
    textureShieldRight = game->resourceMan.GetTexture("shield_right.png");
    textureGameTitle = game->resourceMan.GetTexture("game_title.png");

    // Load fonts
    //font = game->resourceMan.GetFont("PressStart2P.ttf");

    // Load sounds
    brickCollisionBuffer = game->resourceMan.GetSound("brick_collision.wav");
    paddleCollisionBuffer = game->resourceMan.GetSound("paddle_collision.wav");
    newGameBuffer = game->resourceMan.GetSound("new_game.wav");
    gainPowerupBuffer = game->resourceMan.GetSound("gain_powerup.wav");
    loseLifeBuffer = game->resourceMan.GetSound("lose_life.wav");
    gainPowerupBuffer = game->resourceMan.GetSound("gain_powerup.wav");

    if (!backgroundMusic.openFromFile("data\\bgm\\bgm_action_1.ogg")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: bgm_action_1.ogg", "Arkanoid Error", MB_OK);
        #endif
    }

    // Set up sounds from sound buffers
    brickCollisionSound.setBuffer(brickCollisionBuffer);
    paddleCollisionSound.setBuffer(paddleCollisionBuffer);
    newGameSound.setBuffer(newGameBuffer);
    gainPowerupSound.setBuffer(gainPowerupBuffer);
    loseLifeSound.setBuffer(loseLifeBuffer);

    std::cout << "Resources loaded" << std::endl;

    loadObjects();
}

void PlayState::loadObjects()
{
    std::cout << "Loading PlayState objects..." << std::endl;

    // Set up top border shape
    borderTop.setPosition(20, 30);
    borderTop.setSize({440, brickWidth / 2});
    borderTop.setTexture(&textureBorderTop);
    borderTop.setOrigin(0, 0);

    // Set up left border shape
    borderLeft.setPosition(0, 50);
    borderLeft.setSize({brickWidth / 2, 570});
    borderLeft.setTexture(&textureBorderSide);
    borderLeft.setOrigin(0, 0);

    // Set up right border shape
    borderRight.setPosition(460, 50);
    borderRight.setSize({brickWidth / 2, 570});
    borderRight.setTexture(&textureBorderSide);
    borderRight.setOrigin(0, 0);

    // Set up left border corner shape
    borderCornerLeft.setPosition(0, 30);
    borderCornerLeft.setSize({brickWidth / 2, brickWidth / 2});
    borderCornerLeft.setTexture(&textureBorderCornerLeft);
    borderCornerLeft.setOrigin(0, 0);

    // Set up right border corner shape
    borderCornerRight.setPosition(460, 30);
    borderCornerRight.setSize({brickWidth / 2, brickWidth / 2});
    borderCornerRight.setTexture(&textureBorderCornerRight);
    borderCornerRight.setOrigin(0, 0);

    // Set up left shield shape
    shieldLeft.setPosition(0, 620 - 50);
    shieldLeft.setSize({25, brickWidth / 2});
    shieldLeft.setTexture(&textureShieldLeft);
    shieldLeft.setOrigin(0, 0);

    // Set up right shield shape
    shieldRight.setPosition(455, 620 - 50);
    shieldRight.setSize({25, brickWidth / 2});
    shieldRight.setTexture(&textureShieldRight);
    shieldRight.setOrigin(0, 0);

    // Set up top shadow shape
    shadowTop.setPosition(brickWidth, 11 + brickWidth);
    shadowTop.setSize({420, brickWidth / 2});
    shadowTop.setFillColor(sf::Color(0, 0, 0, 127));
    shadowTop.setOrigin(0, 0);

    // Set up left shadow shape
    shadowLeft.setPosition(brickWidth / 2, 40);
    shadowLeft.setSize({brickWidth / 2, 580});
    shadowLeft.setFillColor(sf::Color(0, 0, 0, 127));
    shadowLeft.setOrigin(0, 0);

    std::cout << "PlayState objects loaded" << std::endl;
}
