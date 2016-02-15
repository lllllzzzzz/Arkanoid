#include "..\stateman\gameEngine.hpp"
#include "playState.hpp"

#include <iostream>
//#include <chrono>

#ifdef _WIN32
//#include <windows.h>
#endif

//const float PlayState::ftStep = 1.f;
//const float PlayState::ftSlice = 1.f;

const sf::Vector2f PlayState::BRICK_SIZE = {40, 20};
const int PlayState::POINTS_BALL = 5;
const int PlayState::POINTS_PROJECTILE = 3;
const int PlayState::DEFAULT_PLAYER_LIVES = 2;
const int PlayState::POWERUP_PROBABILITY = 25;
const int PlayState::TOTAL_NUMBER_OF_POWERUPS = 8;

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
    //shield(25, 580),
    paddle()
    //brickGrid(countBricksX, countBricksY, BRICK_SIZE.x, BRICK_SIZE.y)
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
    m_engine = game;

    hud.Init(m_engine);
    paddle.Init(m_engine);
    shield.Init(m_engine);

    LoadResources();
    LoadObjects();
}

void PlayState::Cleanup()
{

}

void PlayState::Pause()
{

}

void PlayState::Resume()
{

}

void PlayState::HandleEvents()
{
    //std::cout << "PlayState HandleEvents()" << std::endl;
    //backgroundMusic.play();

    //timePoint1 = std::chrono::high_resolution_clock::now();

    sf::Event event;
    while (m_engine->getWindow().pollEvent(event)) {
        if (event.type == sf::Event::LostFocus) {
            PauseGame();
        } else if (event.type == sf::Event::GainedFocus) {
            ResumeGame();
        }

        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S) {
            if (paddle.HasLaser()) {
                projectiles.emplace_back(paddle.x() - (paddle.getSize().x / 2) + 8,
                    paddle.y() - paddle.getSize().y);
                projectiles.emplace_back(paddle.x() + (paddle.getSize().x / 2) - 8,
                    paddle.y() - paddle.getSize().y);

                if (IsSoundEnabled()) {
                    sounds.at(SoundEffect::FIRE_PROJECTILE).play();
                }
            }

        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G) {
            bricks.clear();
        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
            for (auto& brick : bricks) {
                brick.setPos({brick.x(), brick.y() + brick.getHeight()});
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
        m_engine->Quit();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        //m_engine->pushState(/*PausedState::Instance()*/new PausedState());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        if (balls.size() == 1) {
            if (!IsBallLaunched()) {
                LaunchBall();
                balls.front().setVelocity({0, -balls.front().getSpeed()});
            }
        }
    }

    if (!playerLives) {
        NewGame();
    }

    if (bricks.empty()) {
        AddBonusPoints(playerLevel);
        LoadLevel(playerLevel + 1);
    }

    if (!shield.IsEnabled()) {
        // Destroy all missed balls
        for (auto& ball : balls) {
            if (ball.y() > /*shield.top()*/m_engine->getWindowSize().y) {
                ball.destroy();
            }
        }
    }

    if (balls.empty()/* && !bricks.empty()*/) {
        RemovePowerups();
        if (!playerLives--) {
            NewGame();
        }

        balls.emplace_back(m_engine, sf::Vector2f{paddle.x(), paddle.y() - ((paddle.getSize().y / 2) + balls.front().getRadius())});
        DockBall();
        //if (isPlayerPlaying()) {
            if (IsSoundEnabled()) {
                sounds.at(SoundEffect::NEW_GAME).play();
            }
        //}
    }

    if (shield.IsEnabled()) {
        for (auto& ball : balls) {
            TestCollision(shield, ball);
        }
    }

    // Perform collision detection between all balls and bricks
    // O(N*M) algorithm
    for (auto& ball : balls) {
        TestCollision(paddle, ball);
        for (auto& brick : bricks) {
            TestCollision(brick, ball);
        }
    }

    // Perform collision detection between paddle and all powerups
    for (auto& powerup : powerups) {
        TestCollision(paddle, powerup);
    }

    // Perform collision detection between projectiles and all bricks
    for (auto& projectile : projectiles) {
        for (auto& brick : bricks) {
            TestCollision(brick, projectile);
        }
    }

    //auto ftSeconds(ft / 1000.f);
    //auto fps(1.f / ftSeconds);

    //m_engine->getWindow().setTitle(
    //    "FT: " + std::to_string(ft) + "\tFPS: " + to_string(fps));
}

void PlayState::Update()
{
    //std::cout << ftStep << ", " << ftSlice << "\n";
    //std::cout << "PlayState Update()" << std::endl;

    if (!IsGameRunning()) {
        return;
    }

    if (IsBallLaunched()) {
        for (auto& ball : balls) {
            ball.update(m_engine->getWindowSize());
        }
    } else {
        balls.front().setPos({paddle.x() - balls.front().getRadius() / 2, paddle.y() - ((balls.front().getRadius() * 2) + 5)});
    }

    for (auto& powerup : powerups) {
        powerup.update(m_engine->getWindowSize());
        //std::cout << "Updating powerup" << std::endl;
    }

    for (auto& projectile : projectiles) {
        projectile.update(m_engine->getWindowSize());
        //std::cout << "Updating projectile" << std::endl;
    }

    /*currentSlice += lastFt;
    for(; currentSlice >= ftSlice; currentSlice -= ftSlice) {*/
        //ball.update(/*ftStep, */m_engine->getWindowSize());

        paddle.update(/*ftStep, */m_engine->getWindowSize());

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
        balls.emplace_back(m_engine, sf::Vector2f{paddle.x() - balls.back().getRadius() / 2, paddle.y() - 20});
        //balls.back().Init();
        balls.back().setVelocity({0, -32});
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        //bricks.clear();
    }
}

void PlayState::Draw()
{
    //std::cout << "PlayState Draw()" << std::endl;

    if (!IsGameRunning()) {
        return;
    }

    m_engine->getWindow().clear({0, 0, 0});

    // Draw background image
    m_engine->getWindow().draw(background.at(BackgroundObject::BACKGROUND));
    // Draw background shadows
    m_engine->getWindow().draw(background.at(BackgroundObject::SHADOW_TOP));
    m_engine->getWindow().draw(background.at(BackgroundObject::SHADOW_LEFT));

    // Draw bricks
    for (auto& brick : bricks) {
        if (brick.isVisible()) {
            m_engine->getWindow().draw(brick.shape);
            m_engine->getWindow().draw(brick.shadow);
        }
    }

    if (!IsBallLaunched()) {
        balls.front().setPos({paddle.x() - balls.front().getRadius() / 2, paddle.y() - ((balls.front().getRadius() * 2) + 5)});
    }

    // Draw balls
    for (auto& ball : balls) {
        if (!ball.isDestroyed()) {
            m_engine->getWindow().draw(ball.shadow);
            m_engine->getWindow().draw(ball.shape);
        }
    }

    // Draw powerups
    for (auto& powerup : powerups) {
        m_engine->getWindow().draw(powerup.shadow);
        m_engine->getWindow().draw(powerup.shape);
    }

    // Draw projectiles
    for (auto& projectile : projectiles) {
        m_engine->getWindow().draw(projectile.shape);
    }

    // Draw paddle
    m_engine->getWindow().draw(paddle.shadow);
    m_engine->getWindow().draw(paddle.shape);

    if (shield.IsEnabled()) {
        m_engine->getWindow().draw(shield.shape);
    }

    // Draw background objects
    m_engine->getWindow().draw(background.at(BackgroundObject::BORDER_TOP));
    m_engine->getWindow().draw(background.at(BackgroundObject::BORDER_LEFT));
    m_engine->getWindow().draw(background.at(BackgroundObject::BORDER_RIGHT));
    m_engine->getWindow().draw(background.at(BackgroundObject::BORDER_CORNER_LEFT));
    m_engine->getWindow().draw(background.at(BackgroundObject::BORDER_CORNER_RIGHT));
    m_engine->getWindow().draw(background.at(BackgroundObject::SHIELD_LEFT));
    m_engine->getWindow().draw(background.at(BackgroundObject::SHIELD_RIGHT));
    m_engine->getWindow().draw(background.at(BackgroundObject::BORDER_CORNER_RIGHT));

    // Draw HUD
    hud.displayHud(playerScore, highScore, playerLives, playerLevel);

    m_engine->getWindow().display();

    /*timePoint2 = std::chrono::high_resolution_clock::now();
    auto elapsedTime(timePoint2 - timePoint1);
    float ft{std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
                     elapsedTime)
                     .count()};

    lastFt = ft;*/
}

void PlayState::NewGame()
{
    //std::cout << "PlayState newGame()" << std::endl;

    playerLevel = 1;
    highScore = std::max(playerScore, highScore);
    playerScore = 0;
    playerLives = DEFAULT_PLAYER_LIVES;

    GenerateNewBrickGrid(playerLevel);

    paddle.reset();
    //paddle.setPos({initPaddlePos.x, initPaddlePos.y});

    powerups.clear();
    projectiles.clear();

    balls.clear();
    balls.emplace_back(m_engine, sf::Vector2f{m_engine->getWindowSize().x / 2, m_engine->getWindowSize().y - paddle.getPos().y - paddle.getSize().y});
    balls.front().setPos({m_engine->getWindowSize().x, paddle.top() - (balls.front().getRadius() * 2) + 1});

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::NEW_GAME).play();
        backgroundMusic.play();
    }
}

void PlayState::GenerateNewBrickGrid(const int level)
{
    //std::cout << level << std::endl;

    bricks.clear();

    for (auto& gridCell : levels.at(level - 1)) {
        bricks.emplace_back(
            m_engine,
            sf::Vector2f{(gridCell.x + 1) * BRICK_SIZE.x, (gridCell.y * BRICK_SIZE.y) + 125},
            gridCell.colour,
            0);
    }
}

void PlayState::RemovePowerups()
{
    shield.Disable();
    paddle.DisableLaser();
    //paddle.Shrink();
}

void PlayState::AddBonusPoints(const int level)
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

void PlayState::LoadLevel(const int level)
{
    playerLevel = (level < 6) ? level : 1;

    paddle.reset();
    //paddle.setPos({initPaddlePos.x, initPaddlePos.y});

    powerups.clear();
    projectiles.clear();

    DockBall();
    balls.clear();
    balls.emplace_back(m_engine, sf::Vector2f{m_engine->getWindowSize().x / 2, m_engine->getWindowSize().y - paddle.getPos().y - paddle.getSize().y});
    balls.front().setPos({m_engine->getWindowSize().x, paddle.top() - (balls.front().getRadius() * 2) + 1});

    if (/*isPlayerPlaying()*/ 1) {
        if (IsSoundEnabled()) {
            sounds.at(SoundEffect::NEW_GAME).play();
            backgroundMusic.play();
        }
    }

    GenerateNewBrickGrid(playerLevel);
}

template <class T1, class T2>
bool PlayState::IsIntersecting(T1& mA, T2& mB)
{
    //std::cout << "PlayState IsIntersecting()" << std::endl;

    return mA.right() >= mB.left() && mA.left() <= mB.right()
        && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void PlayState::TestCollision(Paddle& mPaddle, Ball& mBall)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (!IsIntersecting(mPaddle, mBall)) {
        return;
    }

    float relativeIntersectX{abs(mPaddle.x() - mBall.x()) - 4};
    float normalisedRelativeIntersectX{(relativeIntersectX / (paddle.getSize().x / 2))};
    float bounceAngle{normalisedRelativeIntersectX * (3 * 3.14) / 12};

    sf::Vector2f newBallVelocity;
    newBallVelocity.x = mBall.getSpeed() * sin(bounceAngle);
    newBallVelocity.y = -mBall.getSpeed() * cos(bounceAngle);
    newBallVelocity.x = (mBall.x() < mPaddle.x()) ? -newBallVelocity.x : newBallVelocity.x;

    mBall.setVelocity(newBallVelocity);

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::PADDLE_COLLISION).play();
    }
}

void PlayState::TestCollision(Shield& mShield, Ball& mBall)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (mBall.bottom() < mShield.top()) {
        return;
    }

    mBall.setVelocity({mBall.getVelocity().x, -mBall.getVelocity().y});

    mShield.Disable();

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::SHIELD_BOUNCE).play();
    }
}

void PlayState::TestCollision(Brick& mBrick, Ball& mBall)
{
    //std::cout << "PlayState testCollision() brick" << std::endl;

    if (!IsIntersecting(mBrick, mBall)) {
        return;
    }

    if (mBrick.GetType() == 0) {
        mBrick.destroy();
    }

    playerScore += POINTS_BALL;

    // Calculate intersections of ball/brick
    float overlapLeft{mBall.right() - mBrick.left()};
    float overlapRight{mBrick.right() - mBall.left()};
    float overlapTop{mBall.bottom() - mBrick.top()};
    float overlapBottom{mBrick.bottom() - mBall.top()};

    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
    float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

    sf::Vector2f newBallVelocity;

    if (abs(minOverlapX) < abs(minOverlapY)) {
        newBallVelocity.x = -mBall.getVelocity().x;
        newBallVelocity.y = (mBrick.y() < mBall.y()) ? abs(mBall.getVelocity().y) : -abs(mBall.getVelocity().y);
    } else {
        newBallVelocity.x = (mBall.x() < mBrick.x()) ? -mBall.getVelocity().x : mBall.getVelocity().x;
        newBallVelocity.y = ballFromTop ? -abs(mBall.getVelocity().y) : abs(mBall.getVelocity().y);
    }

    mBall.setVelocity(newBallVelocity);

    if (mBrick.isDestroyed()) {
        if (!((std::rand() % 100) % POWERUP_PROBABILITY)) {
            powerups.emplace_back(m_engine,
                sf::Vector2f{mBrick.x(), mBrick.y()}, m_engine->resourceMan.GetTexture("powerup_extra_life.png"));
        }
    }

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::BRICK_COLLISION).play();
    }
}

void PlayState::TestCollision(Brick& mBrick, Projectile& mProjectile)
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

void PlayState::TestCollision(Paddle& mPaddle, Powerup& mPowerup)
{
    //std::cout << "PlayState testCollision() paddle" << std::endl;

    if (!IsIntersecting(mPaddle, mPowerup)) {
        return;
    }

    mPowerup.destroy();

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::GAIN_POWERUP).play();
    }

    ApplyPowerup();
}

void PlayState::ApplyPowerup()
{
    int n = std::rand() % TOTAL_NUMBER_OF_POWERUPS;

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
            balls.emplace_back(m_engine, sf::Vector2f{paddle.x() - balls.back().radius() / 2, paddle.y() - 20});
            balls.back().setVelocity({0, -balls.back().getSpeed()});
            break;
        case 4:
            //paddle.setVelocity(paddle.getSpeed() + 2);
            paddle.SpeedUp();
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
                brick.setPos({brick.x(), brick.y() + brick.getHeight()});
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

void PlayState::LoadResources()
{
    // Set up sounds vector
    for (int i{0}; i < 8; i++) {
        sounds.emplace_back(sf::Sound());
    }

    sounds.at(SoundEffect::BRICK_COLLISION).setBuffer(m_engine->resourceMan.GetSound("brick_collision.wav"));
    sounds.at(SoundEffect::LOSE_LIFE).setBuffer(m_engine->resourceMan.GetSound("lose_life.wav"));
    sounds.at(SoundEffect::NEW_GAME).setBuffer(m_engine->resourceMan.GetSound("new_game.wav"));
    sounds.at(SoundEffect::PADDLE_COLLISION).setBuffer(m_engine->resourceMan.GetSound("paddle_collision.wav"));
    sounds.at(SoundEffect::SHIELD_BOUNCE).setBuffer(m_engine->resourceMan.GetSound("shield_bounce.wav"));
    sounds.at(SoundEffect::BRICKS_MOVE_DOWN).setBuffer(m_engine->resourceMan.GetSound("bricks_move_down.wav"));
    sounds.at(SoundEffect::GAIN_POWERUP).setBuffer(m_engine->resourceMan.GetSound("gain_powerup.wav"));
    sounds.at(SoundEffect::FIRE_PROJECTILE).setBuffer(m_engine->resourceMan.GetSound("fire_projectile.wav"));

    if (!backgroundMusic.openFromFile("data\\bgm\\bgm_action_1.ogg")) {
        #ifdef _WIN32
        //MessageBox(NULL, "Error loading file: bgm_action_1.ogg", "Arkanoid Error", MB_OK);
        #endif
    }

    // Set up background vector
    for (int i{0}; i < 10; i++) {
        background.emplace_back(sf::RectangleShape());
    }

    //std::cout << "Resources loaded" << std::endl;

    LoadObjects();
}

void PlayState::LoadObjects()
{
    //std::cout << "Loading PlayState objects..." << std::endl;

    const int HUD_Y = hud.getScoreBackgroundHeight();
    const int WINDOW_X = m_engine->getWindowSize().x;
    const int WINDOW_Y = m_engine->getWindowSize().y;

    // Set up left border corner shape
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setPosition(0, HUD_Y);
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setSize({20, 20});
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setTexture(&m_engine->resourceMan.GetTexture("border_corner_left.png"));
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setOrigin(0, 0);

    const int CORNER_LEFT_X = background.at(BackgroundObject::BORDER_CORNER_LEFT).getSize().x;
    const int CORNER_LEFT_Y = background.at(BackgroundObject::BORDER_CORNER_LEFT).getSize().y;

    // Set up right border corner shape
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setPosition(WINDOW_X - CORNER_LEFT_X, 30);
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setSize({20, 20});
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setTexture(&m_engine->resourceMan.GetTexture("border_corner_right.png"));
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setOrigin(0, 0);

    const int CORNER_RIGHT_X = background.at(BackgroundObject::BORDER_CORNER_RIGHT).getSize().x;
    const int CORNER_RIGHT_Y = background.at(BackgroundObject::BORDER_CORNER_RIGHT).getSize().y;

    // Set up top border shape
    background.at(BackgroundObject::BORDER_TOP).setPosition(CORNER_LEFT_X, HUD_Y);
    background.at(BackgroundObject::BORDER_TOP).setSize({WINDOW_X - CORNER_LEFT_X - CORNER_RIGHT_X, 20});
    background.at(BackgroundObject::BORDER_TOP).setTexture(&m_engine->resourceMan.GetTexture("border_top.png"));
    background.at(BackgroundObject::BORDER_TOP).setOrigin(0, 0);

    const int BORDER_TOP_Y = background.at(BackgroundObject::BORDER_TOP).getSize().y;

    // Set up left border shape
    background.at(BackgroundObject::BORDER_LEFT).setPosition(0, HUD_Y + CORNER_LEFT_Y);
    background.at(BackgroundObject::BORDER_LEFT).setSize({20, WINDOW_Y - HUD_Y - CORNER_LEFT_Y});
    background.at(BackgroundObject::BORDER_LEFT).setTexture(&m_engine->resourceMan.GetTexture("border_side.png"));
    background.at(BackgroundObject::BORDER_LEFT).setOrigin(0, 0);

    // Set up right border shape
    background.at(BackgroundObject::BORDER_RIGHT).setPosition(WINDOW_X - CORNER_LEFT_X, HUD_Y + CORNER_RIGHT_Y);
    background.at(BackgroundObject::BORDER_RIGHT).setSize({20, WINDOW_Y - HUD_Y - CORNER_RIGHT_Y});
    background.at(BackgroundObject::BORDER_RIGHT).setTexture(&m_engine->resourceMan.GetTexture("border_side.png"));
    background.at(BackgroundObject::BORDER_RIGHT).setOrigin(0, 0);

    // Set up left shield shape
    background.at(BackgroundObject::SHIELD_LEFT).setPosition(0, WINDOW_Y - 50);
    background.at(BackgroundObject::SHIELD_LEFT).setSize({25, 20});
    background.at(BackgroundObject::SHIELD_LEFT).setTexture(&m_engine->resourceMan.GetTexture("shield_left.png"));
    background.at(BackgroundObject::SHIELD_LEFT).setOrigin(0, 0);

    // Set up right shield shape
    background.at(BackgroundObject::SHIELD_RIGHT).setPosition(WINDOW_X - 25, WINDOW_Y - 50);
    background.at(BackgroundObject::SHIELD_RIGHT).setSize({25, 20});
    background.at(BackgroundObject::SHIELD_RIGHT).setTexture(&m_engine->resourceMan.GetTexture("shield_right.png"));
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
    background.at(BackgroundObject::BACKGROUND).setTexture(&m_engine->resourceMan.GetTexture("hexagon_pattern.png"));
    background.at(BackgroundObject::BACKGROUND).setOrigin(0, 0);

    //std::cout << "PlayState objects loaded" << std::endl;
}
