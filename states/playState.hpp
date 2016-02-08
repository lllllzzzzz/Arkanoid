#include "..\stateman\gameEngine.hpp"
#include "playState.hpp"

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
    playerLevel(1),
    m_isPlayerPlaying(true),
    shield(25, 580),
    paddle(100, 400, 150/*initPaddleX, initPaddleY, paddleWidth, paddleHeight, paddleColour*/)
    //brickGrid(countBricksX, countBricksY, brickWidth, brickHeight)
    //lastFt{0.f},
    //currentSlice{0.f}
{
    //std::cout << "PlayState constructor" << std::endl;

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

    sf::Event event;
    while (game->getWindow().pollEvent(event)) {
        if (event.type == sf::Event::LostFocus) {
            pauseGame();
        } else if (event.type == sf::Event::GainedFocus) {
            resumeGame();
        }

        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S) {
            if (isSoundEnabled()) {
                shootProjectileSound.play();
            }

            projectiles.emplace_back(paddle.x() - (paddle.getWidth() / 2) + 8,
                paddle.y() - paddle.getHeight());
            projectiles.emplace_back(paddle.x() + (paddle.getWidth() / 2) - 8,
                paddle.y() - paddle.getHeight());
        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G) {
            bricks.clear();
        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
            for (auto& brick : bricks) {
                brick.setPos(brick.x(), brick.y() + brick.getHeight());
                if (isSoundEnabled()) {
                    bricksMoveDownSound.play();
                }
            }
        }
    }

    if (!isGameRunning()) {
        return;
    }

    // Handle player input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        game->Quit();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        game->pushState(/*PausedState::Instance()*/new PausedState());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {

        //if (!keyAlreadyPressed) {

            //keyAlreadyPressed = true;

            if (balls.size() == 1) {
                if (!isBallLaunched()) {
                    launchBall();
                    balls.front().setVelocity(0, -balls.front().getVelocity());
                }
            }

            /*if (isSoundEnabled()) {
                shootProjectileSound.play();
            }

            projectiles.emplace_back(paddle.x() - (paddle.getWidth() / 2) + 8,
                paddle.y() - paddle.getHeight());
            projectiles.emplace_back(paddle.x() + (paddle.getWidth() / 2) - 8,
                paddle.y() - paddle.getHeight());*/
            //}

    }

    /*if (sf::Keyboard::isKeyReleased(sf::Keyboard::Key::S)) {
        keyAlreadyPressed = false;
    }*/

    if (!numLives) {
        newGame(game);
    }

    if (bricks.empty()) {
        //if (powerups.empty()) {
            loadLevel(game, playerLevel + 1);
        //}
        //balls.clear();
    }

    if (!shield.IsEnabled()) {
        // Destroy all missed balls
        for (auto& ball : balls) {
            if (ball.y() > shield.top()) {
                ball.destroy();
            }
        }
    }

    if (balls.empty()/* && !bricks.empty()*/) {
        if (!numLives--) {
            newGame(game);
        }

        balls.emplace_back(game ,paddle.x(), paddle.y() - ((paddle.getHeight() / 2) + balls.front().getRadius())/*, ballRadius, ballColour, textureBall*/);
        dockBall();
        //if (isPlayerPlaying()) {
            if (isSoundEnabled()) {
                loseLifeSound.play();
            }
        //}
    }

    if (shield.IsEnabled()) {
        for (auto& ball : balls) {
            testCollision(shield, ball);
        }
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
        testCollision(game, paddle, powerup);
    }

    // Perform collision detection between projectiles and all bricks
    for (auto& projectile : projectiles) {
        for (auto& brick : bricks) {
            testCollision(brick, projectile);
        }
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

    if (isBallLaunched()) {
        for (auto& ball : balls) {
            ball.update(game->getWindowWidth(), game->getWindowHeight());
        }
    } else {
        balls.front().setPos(paddle.x() - balls.front().getRadius() / 2, paddle.y() - ((balls.front().getRadius() * 2) + 3));
    }

    for (auto& powerup : powerups) {
        powerup.update(game->getWindowWidth(), game->getWindowHeight());
        //std::cout << "Updating powerup" << std::endl;
    }

    for (auto& projectile : projectiles) {
        projectile.update(game->getWindowWidth(), game->getWindowHeight());
        //std::cout << "Updating projectile" << std::endl;
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

        // Remove destroyed projectiles from projectiles vector
        projectiles.erase(
            remove_if(begin(projectiles), end(projectiles),
                [](const Projectile& mProjectile) { return mProjectile.isDestroyed(); }),
            end(projectiles)
        );
    //}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        launchBall();
        balls.emplace_back(game, paddle.x() - balls.back().getRadius() / 2, paddle.y() - 20);
        //balls.back().Init(game);
        balls.back().setVelocity(0, -32);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        //bricks.clear();
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

    if (shield.IsEnabled()) {
        game->getWindow().draw(shield.shape);
    }

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

    // Draw background
    game->getWindow().draw(borderRight);
    game->getWindow().draw(shieldRight);

    // Draw powerups
    for (auto& powerup : powerups) {
        game->getWindow().draw(powerup.shadow);
        game->getWindow().draw(powerup.shape);
    }

    // Draw projectiles
    for (auto& projectile : projectiles) {
        game->getWindow().draw(projectile.shape);
    }

    // Draw paddle
    game->getWindow().draw(paddle.shadow);
    game->getWindow().draw(paddle.shape);

    // Draw HUD
    hud.displayHud(&game->getWindow(), playerScore, highScore, numLives, playerLevel, isPlayerPlaying());

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

    playerLevel = 1;
    highScore = std::max(playerScore, highScore);
    playerScore = 0;
    numLives = numLivesDefault;

    paddle.reset();

    generateNewBrickGrid(game, playerLevel);
    //playBrickGridAnimation(game, countBricksX, countBricksY);

    paddle.setPos(initPaddleX, initPaddleY);

    powerups.clear();
    projectiles.clear();

    balls.clear();
    balls.emplace_back(game, initBallX, initBallY/*, ballRadius, ballColour, textureBall*/);
    balls.front().setPos(initPaddleX, paddle.top() - (balls.front().getRadius() * 2) + 1);

    if (isSoundEnabled()) {
        newGameSound.play();
        backgroundMusic.play();
    }
}

void PlayState::generateNewBrickGrid(GameEngine *game, const int level)
{
    std::cout << level << std::endl;

    bricks.clear();

    for (int i{0}; i < levels.at(level - 1).size(); i++) {
        bricks.emplace_back(
            (levels.at(level - 1).at(i).x + 1) * brickWidth,
            (levels.at(level - 1).at(i).y * brickHeight) + 150,
            levels.at(level - 1).at(i).colour, textureBrick,
            0);
        //bricks.back().Init(game);
    }
}

void PlayState::loadLevel(GameEngine *game, const int level)
{
    playerLevel = (level < 6) ? level : 1;

    paddle.setPos(initPaddleX, initPaddleY);

    powerups.clear();
    projectiles.clear();

    dockBall();
    balls.clear();
    balls.emplace_back(game, initBallX, initBallY/*, ballRadius, ballColour*//*, textureBall*/);
    balls.front().setPos(initPaddleX, paddle.top() - (balls.front().getRadius() * 2) + 1);

    if (isPlayerPlaying()) {
        if (isSoundEnabled()) {
            newGameSound.play();
            backgroundMusic.play();
        }
    }

    generateNewBrickGrid(game, playerLevel);
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

void PlayState::testCollision(Shield& mShield, Ball& mBall)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (mBall.bottom() < mShield.top()) {
        return;
    }

    mBall.setVelocity(mBall.getVelocityX(), -mBall.getVelocityY());

    mShield.Disable();

    if (isSoundEnabled()) {
        shieldBounceSound.play();
    }
}

void PlayState::testCollision(Brick& mBrick, Ball& mBall)
{
    //std::cout << "PlayState testCollision() brick" << std::endl;

    if (!isIntersecting(mBrick, mBall)) {
        return;
    }

    if (mBrick.GetType() == 0) {
        mBrick.destroy();
    }

    playerScore += 5;

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

    if (mBrick.isDestroyed()) {
        if (!((std::rand() % 100) % powerupProbability)) {
            powerups.emplace_back(
                mBrick.x(), mBrick.y(), texturePowerup);
        }
    }
    
    if (isSoundEnabled()) {
        brickCollisionSound.play();
    }
}

void PlayState::testCollision(Brick& mBrick, Projectile& mProjectile)
{
    //std::cout << "PlayState testCollision() brick" << std::endl;

    if (!isIntersecting(mBrick, mProjectile)) {
        return;
    }

    mBrick.destroy();
    mProjectile.destroy();

    playerScore += 3;

    if (isSoundEnabled()) {
        brickCollisionSound.play();
    }
}

void PlayState::testCollision(GameEngine *game, Paddle& mPaddle, Powerup& mPowerup)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (!isIntersecting(mPaddle, mPowerup)) {
        return;
    }

    mPowerup.destroy();

    int n = std::rand() % 7;
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
            balls.emplace_back(game, paddle.x() - balls.back().radius() / 2, paddle.y() - 20);
            balls.back().setVelocity(0, -balls.back().getSpeed());
            break;
        case 4:
            paddle.setVelocity(paddle.getVelocity() + 2);
            break;
        case 5:
            shield.Enable();
            break;
        case 6:
            shield.Enable();
            for (auto& brick : bricks) {
                brick.setPos(brick.x(), brick.y() + brick.getHeight());
            }
            if (isSoundEnabled()) {
                bricksMoveDownSound.play();
            }
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
    //textureBall = game->resourceMan.GetTexture("ball.png");
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
    shootProjectileBuffer = game->resourceMan.GetSound("fire_projectile.wav");
    shieldBounceBuffer = game->resourceMan.GetSound("shield_bounce.wav");
    bricksMoveDownBuffer = game->resourceMan.GetSound("bricks_move_down.wav");

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
    shootProjectileSound.setBuffer(shootProjectileBuffer);
    shieldBounceSound.setBuffer(shieldBounceBuffer);
    bricksMoveDownSound.setBuffer(bricksMoveDownBuffer);

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
