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

const int PlayState::initPaddleX = 480 / 2;
const int PlayState::initPaddleY = 620 - 70;
const int PlayState::paddleWidth = 70;
const int PlayState::paddleHeight = 12;
const sf::Color PlayState::paddleColour = sf::Color(255, 255, 255);

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

PlayState::PlayState() :
    m_isSoundEnabled(true),
    highScore(0),
    numLives(0),
    level(1),
    m_isPlayerPlaying(false),
    paddle(initPaddleX, initPaddleY, paddleWidth, paddleHeight, paddleColour/*, &texturePaddle*/)
    //brickGrid(countBricksX, countBricksY, brickWidth, brickHeight)
    //lastFt{0.f},
    //currentSlice{0.f}
{
    //std::cout << "PlayState constructor" << std::endl;

    //newGame();

    loadResources();
    setupBackground();

    /*lastFt = 0.f;
    currentSlice = 0.f;*/

    std::srand(time(0));
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        /*if (isGameRunning()) {
            pauseGame();
        } else {
            resumeGame();
        }*/

        // TODO: IMPLEMENT DEBOUNCING
        pauseGame();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        /*if (isGameRunning()) {
            pauseGame();
        } else {
            resumeGame();
        }*/

        // TODO: IMPLEMENT DEBOUNCING
        resumeGame();
    }

    if (!isGameRunning()) {
        return;
    }

    if (!numLives) {
        newGame(game);
    }

    if (bricks.empty()) {
        loadNextLevel();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            //game->changeState(new MenuState());
            //game->pushState(new MenuState());
            game->Quit();
        /*} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            game->Quit();*/
        }

    if (isPlayerPlaying()) {
        // Accept player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            //game->changeState(new MenuState());
            //game->pushState(new MenuState());
            game->Quit();
        /*} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            game->Quit();*/
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            if (balls.size() == 1) {
                if (!balls.front().isLaunched()) {
                    balls.front().launch();
                    balls.front().setVelocity(0, -balls.front().getVelocity());
                }
            }
        }
    } else {
        // AI
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            playerStartPlaying();
            paddle.setVelocity(8);
            newGame(game);
        }

        if (!isPlayerPlaying()) {
            for (auto &ball : balls) {
                if (!ball.isLaunched()) {
                    ball.launch();
                }

                if (/*ball.right() < paddle.left()*/ball.right() < paddle.x() - brickWidth / 2) {
                    if (paddle.left() > 25) {
                        paddle.moveLeft();
                    }
                } else if (/*ball.left() > paddle.right()*/ball.left() > paddle.x() + brickWidth / 2) {
                    if (paddle.right() < 455) {
                        paddle.moveRight();
                    }
                }
            }
        }
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
    // O(M*N) algorithm
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

    //game->window.setTitle(
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
        //std::cout << "New ball" << std::endl;
        balls.back().launch();
        balls.emplace_back(paddle.x() - balls.back().getRadius() / 2, paddle.y() - 20, ballRadius, ballColour, textureBall);
        balls.back().launch();
        balls.back().setVelocity(0, -32);

        /*if (paddle.getWidth() < 430) {
            paddle.setSize(paddle.getWidth() + 10, paddleHeight);
            paddle.setPos(paddle.x() + 0, paddle.y());
            paddle.shape.setOrigin(paddle.getWidth() / 2, paddle.getHeight() / 2);
            balls.front().setPos(balls.front().x() - 0, balls.front().y());
        }*/

    //}
    //playerScore += 5;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
            //std::cout << "New ball" << std::endl;
            bricks.clear();
        //}
        //playerScore += 5;
    }
}

void PlayState::Draw(GameEngine *game)
{
    //std::cout << "PlayState Draw()" << std::endl;

    if (!isGameRunning()) {
        return;
    }

    game->window.clear(sf::Color(0, 0, 98));

    game->window.draw(shadowTop);
    game->window.draw(shadowLeft);
    game->window.draw(borderTop);
    game->window.draw(borderLeft);
    game->window.draw(borderCornerLeft);
    game->window.draw(borderCornerRight);
    game->window.draw(shieldLeft);
    //game->window.draw(shieldRight);
    /*game->window.draw(borderRight);*/

    // Draw bricks
    for (auto& brick : bricks) {
        if (brick.isVisible()) {
            game->window.draw(brick.shape);
            game->window.draw(brick.shadow);
        }
    }

    // Draw balls
    for (auto& ball : balls) {
        if (!ball.isDestroyed()) {
            game->window.draw(ball.shadow);
            game->window.draw(ball.shape);
        }
    }

    // Draw paddle
    game->window.draw(paddle.shadow);
    game->window.draw(paddle.shape);

    game->window.draw(borderRight);
    game->window.draw(shieldRight);

    // Draw powerups
    for (auto& powerup : powerups) {
        game->window.draw(powerup.shape);
    }

    hud.displayHud(&game->window, playerScore, highScore, numLives, level, isPlayerPlaying());

    game->window.display();

    /*timePoint2 = std::chrono::high_resolution_clock::now();
    auto elapsedTime(timePoint2 - timePoint1);
    float ft{std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
                     elapsedTime)
                     .count()};

    lastFt = ft;*/
}

void PlayState::loadResources()
{
    // Load sound resources
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

    if (!newGameBuffer.loadFromFile("data\\sound\\new_game.wav")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: new_game.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    newGameSound.setBuffer(newGameBuffer);

    if (!gainPowerupBuffer.loadFromFile("data\\sound\\gain_powerup.wav")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: gain_powerup.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    gainPowerupSound.setBuffer(gainPowerupBuffer);

    if (!loseLifeBuffer.loadFromFile("data\\sound\\lose_life.wav")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: gain_powerup.wav!", "Arkanoid Error", MB_OK);
        #endif
    }
    loseLifeSound.setBuffer(loseLifeBuffer);

    if (!backgroundMusic.openFromFile("data\\sound\\bgm_action_1.ogg")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: bgm_action_1.ogg", "Arkanoid Error", MB_OK);
        #endif
    }

    // Load sprite/texture resources
    if (!textureBrick.loadFromFile("data\\graphics\\brick.png")) {
    }
    /*if (!texturePaddle.loadFromFile("data\\graphics\\bat_black.png")) {
    }*/
    if (!textureBall.loadFromFile("data\\graphics\\ball.png")) {
    }
    if (!texturePowerup.loadFromFile("data\\graphics\\powerup_extra_life.png")) {
    }
    if (!textureBorderSide.loadFromFile("data\\graphics\\border_side.png")) {
    }
    if (!textureBorderTop.loadFromFile("data\\graphics\\border_top.png")) {
    }
    if (!textureBorderCornerLeft.loadFromFile("data\\graphics\\border_corner_left.png")) {
    }
    if (!textureBorderCornerRight.loadFromFile("data\\graphics\\border_corner_right.png")) {
    }
    if (!textureShieldLeft.loadFromFile("data\\graphics\\shield_left.png")) {
    }
    if (!textureShieldRight.loadFromFile("data\\graphics\\shield_right.png")) {
    }
}

void PlayState::newGame(GameEngine *game)
{
    //std::cout << "PlayState newGame()" << std::endl;

    level = 1;
    highScore = std::max(playerScore, highScore);
    playerScore = 0;
    numLives = numLivesDefault;

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

    for (int iX{0}; iX < numBricksX; iX++) {
        for (int iY{0}; iY < numBricksY; iY++) {
            // Generate random colour for brick
            //std::srand(std::time(0));

            int r = std::rand() % 256;
            int g = std::rand() % 256;
            int b = std::rand() % 256;
            sf::Color colour(r, g, b);

            bricks.emplace_back(
                (iX + 1) * (width + 0) - 0, (iY + 6) * (height + 0),
                width, height, colour, textureBrick);
        }
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
    float normalizedRelativeIntersectionX{(relativeIntersectX / (paddleWidth / 2))};
    float bounceAngle{normalizedRelativeIntersectionX * (4 * 3.14) / 12};

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

    /*if (paddle.getWidth() < 430) {
        paddle.setSize(paddle.getWidth() + 20, paddleHeight);
        paddle.setPos(paddle.x() + 0, paddle.y());
        paddle.shape.setOrigin(paddle.getWidth() / 2, paddle.getHeight() / 2);
        balls.front().setPos(balls.front().x() - 0, balls.front().y());
    }*/

    if (isPlayerPlaying()) {
        playerScore += ballPoints;
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
        //mBall.setVelocity(ballFromLeft ? -mBall.getVelocity() : mBall.getVelocity());

        //mBall.setVelocityX(ballFromLeft ? -mBall.getVelocity() : mBall.getVelocity());
        //int newVelocityX = ballFromLeft ? -mBall.getVelocity() : mBall.getVelocity();
        // Realistic brick physics
        //newVelocityX *= abs(mBrick.x() - mBall.x()) / 20;
        //mBall.setVelocityX(newVelocityX);

        float intersectY = abs(mBall.y() - mBrick.y()) - 4;
        float relativeIntersectY = intersectY;
        float normalizedRelativeIntersectionY = (relativeIntersectY / (brickHeight / 2));
        float bounceAngle = normalizedRelativeIntersectionY * (4 * 3.14) / 12;

        int ballVy = mBall.getVelocity() * sin(bounceAngle);
        int ballVx = mBall.getVelocity() * -cos(bounceAngle);

        //std::cout << "side" << std::endl;

        mBall.setVelocityX(-mBall.getVelocityX());
        mBall.setVelocityY(mBrick.y() < mBall.y() ? abs(mBall.getVelocityY()) : -abs(mBall.getVelocityY()));

        //mBall.setVelocityX(ballVx);
        //mBall.setVelocityY(ballVy);
        //mBall.setVelocityY(mBrick.y() < mBall.y() ? abs(mBall.getVelocityY()) : -abs(mBall.getVelocityY()));
    } else {
       /* if (ballFromTop) {
            mBall.setVelocityY(mBall.getVelocityY() ? -mBall.getVelocityY() : mBall.getVelocityY());
        } else {
            mBall.setVelocityY(mBall.getVelocityY() ? mBall.getVelocityY() : -mBall.getVelocityY());
        }*/
        //mBall.setVelocityY(ballFromTop ? -mBall.getVelocity() : mBall.getVelocity());

        float intersectX = abs(mBall.x() - mBrick.x()) - 4;
        float relativeIntersectX = /*paddle.x() - intersectX*/intersectX;
        float normalizedRelativeIntersectionX = (relativeIntersectX / (brickWidth / 2));
        float bounceAngle = normalizedRelativeIntersectionX * (4 * 3.14) / 12;

        int ballVx = mBall.getVelocity() * sin(bounceAngle);
        int ballVy = mBall.getVelocity() * cos(bounceAngle);

        //std::cout << "top/bottom" << std::endl;

        //mBall.setVelocityX(ballVx);
        mBall.setVelocityX(mBall.x() < mBrick.x() ? -mBall.getVelocityX() : mBall.getVelocityX());
        mBall.setVelocityY(ballFromTop ? -abs(mBall.getVelocityY()) : abs(mBall.getVelocityY()));

        //mBall.setVelocityY(ballVx, -ballVy);
    }

    std::cout << "x :" << mBall.getVelocityX() << "    y : " << mBall.getVelocityY() << std::endl;

    if (!((std::rand() % 100) % powerupProbability)) {
        //int speed = (std::rand() % 5) + 1;
        //std::cout << speed << std::endl;
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
            paddle.setSize(paddle.getWidth() + 15, paddleHeight);
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
    /*std::vector<int> nums;
    for (int i{0}; i < numBricksX * numBricksY; i++) {
        nums.push_back(i);
    }

    std::random_shuffle(nums.begin(), nums.end());

    for (int i{0}; i < nums.size(); i++) {
        std::cout << nums.at(i) << std::endl;
    }*/

    for (int i{0}; i < numBricksY; i += 1) {
        for (int j{i}; j < numBricksX; j++) {
            bricks.at(j).setVisibility(true);
            //std::cout << j << ", " << i << std::endl;
        }

        game->window.clear(sf::Color::Black);

        game->window.draw(paddle.shape);

        for (auto& brick : bricks) {
            if (brick.isVisible()) {
                game->window.draw(brick.shape);
                //game->window.draw(brick.shadow);
                return;
            }
        }

        game->window.display();
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

void PlayState::setupBackground()
{
    borderTop.setPosition(20, 30);
    borderTop.setSize({440, brickWidth / 2});
    borderTop.setTexture(&textureBorderTop);
    borderTop.setOrigin(0, 0);

    borderLeft.setPosition(0, 50);
    borderLeft.setSize({brickWidth / 2, 570});
    borderLeft.setTexture(&textureBorderSide);
    borderLeft.setOrigin(0, 0);

    borderRight.setPosition(460, 50);
    borderRight.setSize({brickWidth / 2, 570});
    borderRight.setTexture(&textureBorderSide);
    borderRight.setOrigin(0, 0);

    borderCornerLeft.setPosition(0, 30);
    borderCornerLeft.setSize({brickWidth / 2, brickWidth / 2});
    borderCornerLeft.setTexture(&textureBorderCornerLeft);
    borderCornerLeft.setOrigin(0, 0);

    borderCornerRight.setPosition(460, 30);
    borderCornerRight.setSize({brickWidth / 2, brickWidth / 2});
    borderCornerRight.setTexture(&textureBorderCornerRight);
    borderCornerRight.setOrigin(0, 0);

    shieldLeft.setPosition(0, 620 - 50);
    shieldLeft.setSize({25, brickWidth / 2});
    shieldLeft.setTexture(&textureShieldLeft);
    shieldLeft.setOrigin(0, 0);

    shieldRight.setPosition(455, 620 - 50);
    shieldRight.setSize({25, brickWidth / 2});
    shieldRight.setTexture(&textureShieldRight);
    shieldRight.setOrigin(0, 0);

    shadowTop.setPosition(brickWidth, 11 + brickWidth);
    shadowTop.setSize({420, brickWidth / 2});
    shadowTop.setFillColor(sf::Color(0, 0, 0, 127));
    shadowTop.setOrigin(0, 0);

    shadowLeft.setPosition(brickWidth / 2, 40);
    shadowLeft.setSize({brickWidth / 2, 580});
    shadowLeft.setFillColor(sf::Color(0, 0, 0, 127));
    shadowLeft.setOrigin(0, 0);
}
