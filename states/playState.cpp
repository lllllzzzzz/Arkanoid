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

const int PlayState::ballPoints = 5;
const int PlayState::projectilePoints = 3;

const int PlayState::playerLivesDefault = 2;

const int PlayState::powerupProbability = 25;

PlayState PlayState::m_PlayState;

//using namespace Arkanoid;

PlayState::PlayState() :
    levels{ level1, level2, level3, level4, level5 },
    highScore(0),
    playerLives(0),
    playerLevel(1),
    m_isPlayerPlaying(true),
    m_isSoundEnabled(true),
    m_isGameRunning(true),
    m_isBallLaunched(false),
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
    LoadObjects(game);
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
            PauseGame();
        } else if (event.type == sf::Event::GainedFocus) {
            ResumeGame();
        }

        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S) {
            if (paddle.HasLaser()) {
                projectiles.emplace_back(paddle.x() - (paddle.getWidth() / 2) + 8,
                    paddle.y() - paddle.getHeight());
                projectiles.emplace_back(paddle.x() + (paddle.getWidth() / 2) - 8,
                    paddle.y() - paddle.getHeight());

                if (IsSoundEnabled()) {
                    sounds.at(SoundEffect::FIRE_PROJECTILE).play();
                }
            }

        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G) {
            bricks.clear();
        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
            for (auto& brick : bricks) {
                brick.setPos(brick.x(), brick.y() + brick.getHeight());
                if (IsSoundEnabled()) {
                    sounds.at(SoundEffect::BRICKS_MOVE_DOWN).play();
                }
            }
        }
    }

    if (!IsGameRunning()) {
        return;
    }

    /*LaunchBall();

    for (auto& ball : balls) {
        if (ball.right() < paddle.left()) {
            paddle.MoveLeft();
        } else if (ball.left() > paddle.right()) {
            paddle.MoveRight();
        }
    }*/

    // Handle player input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        game->Quit();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        game->pushState(/*PausedState::Instance()*/new PausedState());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {

        //if (!keyAlreadyPressed) {

            //keyAlreadyPressed = true;

            if (balls.size() == 1) {
                if (!IsBallLaunched()) {
                    LaunchBall();
                    balls.front().setVelocity(0, -balls.front().getVelocity());
                }
            }

            /*if (IsSoundEnabled()) {
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

    if (!playerLives) {
        NewGame(game);
    }

    if (bricks.empty()) {
        //if (powerups.empty()) {
            AddBonusPoints(game, playerLevel);
            LoadLevel(game, playerLevel + 1);
        //}
        //balls.clear();
    }

    if (!shield.IsEnabled()) {
        // Destroy all missed balls
        for (auto& ball : balls) {
            if (ball.y() > /*shield.top()*/game->getWindowHeight()) {
                ball.destroy();
            }
        }
    }

    if (balls.empty()/* && !bricks.empty()*/) {
        RemovePowerups(game);
        if (!playerLives--) {
            NewGame(game);
        }

        balls.emplace_back(game ,paddle.x(), paddle.y() - ((paddle.getHeight() / 2) + balls.front().getRadius()));
        DockBall();
        //if (isPlayerPlaying()) {
            if (IsSoundEnabled()) {
                sounds.at(SoundEffect::NEW_GAME).play();
            }
        //}
    }

    if (shield.IsEnabled()) {
        for (auto& ball : balls) {
            TestCollision(game, shield, ball);
        }
    }

    // Perform collision detection between all balls and bricks
    // O(N*M) algorithm
    for (auto& ball : balls) {
        TestCollision(game, paddle, ball);
        for (auto& brick : bricks) {
            TestCollision(game, brick, ball);
        }
    }

    // Perform collision detection between paddle and all powerups
    for (auto& powerup : powerups) {
        TestCollision(game, paddle, powerup);
    }

    // Perform collision detection between projectiles and all bricks
    for (auto& projectile : projectiles) {
        for (auto& brick : bricks) {
            TestCollision(game, brick, projectile);
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

    if (!IsGameRunning()) {
        return;
    }

    if (IsBallLaunched()) {
        for (auto& ball : balls) {
            ball.update(game->getWindowWidth(), game->getWindowHeight());
        }
    } else {
        balls.front().setPos(paddle.x() - balls.front().getRadius() / 2, paddle.y() - ((balls.front().getRadius() * 2) + 5));
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
        LaunchBall();
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

    if (!IsGameRunning()) {
        return;
    }

    game->getWindow().clear({0, 0, 0});

    // Draw background image
    game->getWindow().draw(background.at(BackgroundObject::BACKGROUND));
    // Draw background shadows
    game->getWindow().draw(background.at(BackgroundObject::SHADOW_TOP));
    game->getWindow().draw(background.at(BackgroundObject::SHADOW_LEFT));

    // Draw bricks
    for (auto& brick : bricks) {
        if (brick.isVisible()) {
            game->getWindow().draw(brick.shape);
            game->getWindow().draw(brick.shadow);
        }
    }

    if (!IsBallLaunched()) {
        balls.front().setPos(paddle.x() - balls.front().getRadius() / 2, paddle.y() - ((balls.front().getRadius() * 2) + 5));
    }

    // Draw balls
    for (auto& ball : balls) {
        if (!ball.isDestroyed()) {
            game->getWindow().draw(ball.shadow);
            game->getWindow().draw(ball.shape);
        }
    }

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

    if (shield.IsEnabled()) {
        game->getWindow().draw(shield.shape);
    }

    // Draw background objects
    game->getWindow().draw(background.at(BackgroundObject::BORDER_TOP));
    game->getWindow().draw(background.at(BackgroundObject::BORDER_LEFT));
    game->getWindow().draw(background.at(BackgroundObject::BORDER_RIGHT));
    game->getWindow().draw(background.at(BackgroundObject::BORDER_CORNER_LEFT));
    game->getWindow().draw(background.at(BackgroundObject::BORDER_CORNER_RIGHT));
    game->getWindow().draw(background.at(BackgroundObject::SHIELD_LEFT));
    game->getWindow().draw(background.at(BackgroundObject::SHIELD_RIGHT));
    game->getWindow().draw(background.at(BackgroundObject::BORDER_CORNER_RIGHT));

    // Draw HUD
    hud.displayHud(game, playerScore, highScore, playerLives, playerLevel);

    game->getWindow().display();

    /*timePoint2 = std::chrono::high_resolution_clock::now();
    auto elapsedTime(timePoint2 - timePoint1);
    float ft{std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
                     elapsedTime)
                     .count()};

    lastFt = ft;*/
}

void PlayState::NewGame(GameEngine *game)
{
    //std::cout << "PlayState newGame()" << std::endl;

    playerLevel = 1;
    highScore = std::max(playerScore, highScore);
    playerScore = 0;
    playerLives = playerLivesDefault;

    GenerateNewBrickGrid(game, playerLevel);

    paddle.reset();
    paddle.setPos(initPaddleX, initPaddleY);

    powerups.clear();
    projectiles.clear();

    balls.clear();
    balls.emplace_back(game, initBallX, initBallY);
    balls.front().setPos(initPaddleX, paddle.top() - (balls.front().getRadius() * 2) + 1);

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::NEW_GAME).play();
        backgroundMusic.play();
    }
}

void PlayState::GenerateNewBrickGrid(GameEngine *game, const int level)
{
    std::cout << level << std::endl;

    bricks.clear();

    for (int i{0}; i < levels.at(level - 1).size(); i++) {
        bricks.emplace_back(
            game,
            (levels.at(level - 1).at(i).x + 1) * brickWidth,
            (levels.at(level - 1).at(i).y * brickHeight) + 125,
            levels.at(level - 1).at(i).colour,
            0);
    }
}

void PlayState::RemovePowerups(GameEngine *game)
{
    shield.Disable();
    paddle.DisableLaser();
    paddle.Shrink();
}

void PlayState::AddBonusPoints(GameEngine *game, const int level)
{
    int bonusPoints = 0;

    if (shield.IsEnabled()) {
        bonusPoints += 50;
    }

    if (paddle.HasLaser()) {
        bonusPoints += 40;
    }

    if (paddle.IsExpanded()) {
        bonusPoints += 25;
    }

    bonusPoints += playerLives * 12;
    bonusPoints += level * 15;

    playerScore += bonusPoints;
}

void PlayState::LoadLevel(GameEngine *game, const int level)
{
    playerLevel = (level < 6) ? level : 1;

    paddle.reset();
    paddle.setPos(initPaddleX, initPaddleY);

    powerups.clear();
    projectiles.clear();

    DockBall();
    balls.clear();
    balls.emplace_back(game, initBallX, initBallY);
    balls.front().setPos(initPaddleX, paddle.top() - (balls.front().getRadius() * 2) + 1);

    if (/*isPlayerPlaying()*/ 1) {
        if (IsSoundEnabled()) {
            sounds.at(SoundEffect::NEW_GAME).play();
            backgroundMusic.play();
        }
    }

    GenerateNewBrickGrid(game, playerLevel);
}

template <class T1, class T2>
bool PlayState::IsIntersecting(T1& mA, T2& mB)
{
    //std::cout << "PlayState IsIntersecting()" << std::endl;

    return mA.right() >= mB.left() && mA.left() <= mB.right()
        && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void PlayState::TestCollision(GameEngine *game, Paddle& mPaddle, Ball& mBall)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (!IsIntersecting(mPaddle, mBall)) {
        return;
    }

    float intersectX{abs(mPaddle.x() - mBall.x()) - 4};
    float relativeIntersectX{/*paddle.x() - intersectX*/intersectX};
    float normalizedRelativeIntersectionX{(relativeIntersectX / (paddle.getWidth() / 2))};
    float bounceAngle{normalizedRelativeIntersectionX * (3 * 3.14) / 12};

    /*std::cout << intersectX << std::endl;
    std::cout << relativeIntersectX << std::endl;
    std::cout << normalizedRelativeIntersectionX << std::endl;
    std::cout << bounceAngle << std::endl << std::endl;*/

    int ballVx{mBall.getVelocity() * sin(bounceAngle)};
    int ballVy{mBall.getVelocity() * cos(bounceAngle)};

    mBall.setVelocityX(ballVx);
    mBall.setVelocityY(ballVy);

    ballVx = (mBall.x() < mPaddle.x()) ? -ballVx : ballVx;

    //std::cout << ballVx << ", " << ballVy << std::endl;

    mBall.setVelocity(ballVx, -ballVy);

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::PADDLE_COLLISION).play();
    }
}

void PlayState::TestCollision(GameEngine *game, Shield& mShield, Ball& mBall)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (mBall.bottom() < mShield.top()) {
        return;
    }

    mBall.setVelocity(mBall.getVelocityX(), -mBall.getVelocityY());

    mShield.Disable();

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::SHIELD_BOUNCE).play();
    }
}

void PlayState::TestCollision(GameEngine *game, Brick& mBrick, Ball& mBall)
{
    //std::cout << "PlayState testCollision() brick" << std::endl;

    if (!IsIntersecting(mBrick, mBall)) {
        return;
    }

    if (mBrick.GetType() == 0) {
        mBrick.destroy();
    }

    playerScore += ballPoints;

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
                mBrick.x(), mBrick.y(), game->resourceMan.GetTexture("powerup_extra_life.png"));
        }
    }

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::BRICK_COLLISION).play();
    }
}

void PlayState::TestCollision(GameEngine *game, Brick& mBrick, Projectile& mProjectile)
{
    //std::cout << "PlayState testCollision() brick" << std::endl;

    if (!IsIntersecting(mBrick, mProjectile)) {
        return;
    }

    mBrick.destroy();
    mProjectile.destroy();

    playerScore += 3;

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::BRICK_COLLISION).play();
    }
}

void PlayState::TestCollision(GameEngine *game, Paddle& mPaddle, Powerup& mPowerup)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (!IsIntersecting(mPaddle, mPowerup)) {
        return;
    }

    mPowerup.destroy();

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::GAIN_POWERUP).play();
    }

    ApplyPowerup(game);
}

void PlayState::ApplyPowerup(GameEngine *game)
{
    int n = std::rand() % 8;

    switch (n) {
        case 0:
            if (playerLives < 4) {
                playerLives++;
            } else {
                playerScore += 30;
            }

            break;
        case 1:
            if (!paddle.IsExpanded()) {
                paddle.Expand();
            } else {
                playerScore += 30;
            }

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
            if (!shield.IsEnabled()) {
                shield.Enable();
            } else {
                playerScore += 30;
            }

            break;
        case 6:
            for (auto& brick : bricks) {
                brick.setPos(brick.x(), brick.y() + brick.getHeight());
            }
            if (IsSoundEnabled()) {
                sounds.at(SoundEffect::BRICKS_MOVE_DOWN).play();
            }
            break;
        case 7:
            paddle.EnableLaser();
            break;
        default:
            break;
    }
}

void PlayState::LoadResources(GameEngine *game)
{
    // Set up sounds vector
    for (int i{0}; i < 8; i++) {
        sounds.emplace_back(sf::Sound());
    }

    sounds.at(SoundEffect::BRICK_COLLISION).setBuffer(game->resourceMan.GetSound("brick_collision.wav"));
    sounds.at(SoundEffect::LOSE_LIFE).setBuffer(game->resourceMan.GetSound("lose_life.wav"));
    sounds.at(SoundEffect::NEW_GAME).setBuffer(game->resourceMan.GetSound("new_game.wav"));
    sounds.at(SoundEffect::PADDLE_COLLISION).setBuffer(game->resourceMan.GetSound("paddle_collision.wav"));
    sounds.at(SoundEffect::SHIELD_BOUNCE).setBuffer(game->resourceMan.GetSound("shield_bounce.wav"));
    sounds.at(SoundEffect::BRICKS_MOVE_DOWN).setBuffer(game->resourceMan.GetSound("bricks_move_down.wav"));
    sounds.at(SoundEffect::GAIN_POWERUP).setBuffer(game->resourceMan.GetSound("gain_powerup.wav"));
    sounds.at(SoundEffect::FIRE_PROJECTILE).setBuffer(game->resourceMan.GetSound("fire_projectile.wav"));

    if (!backgroundMusic.openFromFile("data\\bgm\\bgm_action_1.ogg")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: bgm_action_1.ogg", "Arkanoid Error", MB_OK);
        #endif
    }

    // Set up background vector
    for (int i{0}; i < 10; i++) {
        background.emplace_back(sf::RectangleShape());
    }

    std::cout << "Resources loaded" << std::endl;

    LoadObjects(game);
}

void PlayState::LoadObjects(GameEngine *game)
{
    std::cout << "Loading PlayState objects..." << std::endl;

    const int HUD_Y = hud.getScoreBackgroundHeight();
    const int WINDOW_X = game->getWindowWidth();
    const int WINDOW_Y = game->getWindowHeight();

    // Set up left border corner shape
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setPosition(0, HUD_Y);
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setSize({20, 20});
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setTexture(&game->resourceMan.GetTexture("border_corner_left.png"));
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setOrigin(0, 0);

    const int CORNER_LEFT_X = background.at(BackgroundObject::BORDER_CORNER_LEFT).getSize().x;
    const int CORNER_LEFT_Y = background.at(BackgroundObject::BORDER_CORNER_LEFT).getSize().y;

    // Set up right border corner shape
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setPosition(WINDOW_X - CORNER_LEFT_X, 30);
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setSize({20, 20});
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setTexture(&game->resourceMan.GetTexture("border_corner_right.png"));
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setOrigin(0, 0);

    const int CORNER_RIGHT_X = background.at(BackgroundObject::BORDER_CORNER_RIGHT).getSize().x;
    const int CORNER_RIGHT_Y = background.at(BackgroundObject::BORDER_CORNER_RIGHT).getSize().y;

    // Set up top border shape
    background.at(BackgroundObject::BORDER_TOP).setPosition(CORNER_LEFT_X, HUD_Y);
    background.at(BackgroundObject::BORDER_TOP).setSize({WINDOW_X - CORNER_LEFT_X - CORNER_RIGHT_X, 20});
    background.at(BackgroundObject::BORDER_TOP).setTexture(&game->resourceMan.GetTexture("border_top.png"));
    background.at(BackgroundObject::BORDER_TOP).setOrigin(0, 0);

    const int BORDER_TOP_Y = background.at(BackgroundObject::BORDER_TOP).getSize().y;

    // Set up left border shape
    background.at(BackgroundObject::BORDER_LEFT).setPosition(0, HUD_Y + CORNER_LEFT_Y);
    background.at(BackgroundObject::BORDER_LEFT).setSize({20, WINDOW_Y - HUD_Y - CORNER_LEFT_Y});
    background.at(BackgroundObject::BORDER_LEFT).setTexture(&game->resourceMan.GetTexture("border_side.png"));
    background.at(BackgroundObject::BORDER_LEFT).setOrigin(0, 0);

    // Set up right border shape
    background.at(BackgroundObject::BORDER_RIGHT).setPosition(WINDOW_X - CORNER_LEFT_X, HUD_Y + CORNER_RIGHT_Y);
    background.at(BackgroundObject::BORDER_RIGHT).setSize({20, WINDOW_Y - HUD_Y - CORNER_RIGHT_Y});
    background.at(BackgroundObject::BORDER_RIGHT).setTexture(&game->resourceMan.GetTexture("border_side.png"));
    background.at(BackgroundObject::BORDER_RIGHT).setOrigin(0, 0);

    // Set up left shield shape
    background.at(BackgroundObject::SHIELD_LEFT).setPosition(0, WINDOW_Y - 50);
    background.at(BackgroundObject::SHIELD_LEFT).setSize({25, 20});
    background.at(BackgroundObject::SHIELD_LEFT).setTexture(&game->resourceMan.GetTexture("shield_left.png"));
    background.at(BackgroundObject::SHIELD_LEFT).setOrigin(0, 0);

    // Set up right shield shape
    background.at(BackgroundObject::SHIELD_RIGHT).setPosition(WINDOW_X - 25, WINDOW_Y - 50);
    background.at(BackgroundObject::SHIELD_RIGHT).setSize({25, 20});
    background.at(BackgroundObject::SHIELD_RIGHT).setTexture(&game->resourceMan.GetTexture("shield_right.png"));
    background.at(BackgroundObject::SHIELD_RIGHT).setOrigin(0, 0);

    // Set up top shadow shape
    background.at(BackgroundObject::SHADOW_TOP).setPosition(CORNER_LEFT_X, HUD_Y + BORDER_TOP_Y);
    background.at(BackgroundObject::SHADOW_TOP).setSize({WINDOW_X - CORNER_LEFT_X - CORNER_RIGHT_X, 20});
    background.at(BackgroundObject::SHADOW_TOP).setFillColor({0, 0, 0, 127});
    background.at(BackgroundObject::SHADOW_TOP).setOrigin(0, 0);

    // Set up left shadow shape
    background.at(BackgroundObject::SHADOW_LEFT).setPosition(20, HUD_Y + BORDER_TOP_Y + 20);
    background.at(BackgroundObject::SHADOW_LEFT).setSize({20, WINDOW_Y - HUD_Y - BORDER_TOP_Y});
    background.at(BackgroundObject::SHADOW_LEFT).setFillColor({0, 0, 0, 127});
    background.at(BackgroundObject::SHADOW_LEFT).setOrigin(0, 0);

    // Set up background shape
    background.at(BackgroundObject::BACKGROUND).setPosition(0, 0);
    background.at(BackgroundObject::BACKGROUND).setSize({WINDOW_X, WINDOW_Y});
    background.at(BackgroundObject::BACKGROUND).setTexture(&game->resourceMan.GetTexture("hexagon_pattern.png"));
    background.at(BackgroundObject::BACKGROUND).setOrigin(0, 0);

    std::cout << "PlayState objects loaded" << std::endl;
}
