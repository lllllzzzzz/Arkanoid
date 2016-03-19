#include "..\stateman\gameEngine.hpp"
#include "playState.hpp"

#include <iostream>
//#include <chrono>

#ifdef _WIN32
#endif

#define M_PI (3.14159265359)

//const float PlayState::ftStep = 1.f;
//const float PlayState::ftSlice = 1.f;

const sf::Color PlayState::BACKGROUND_CLEAR_COLOUR = {0, 0, 0};
const int PlayState::POINTS_BALL = 5;
const int PlayState::POINTS_PROJECTILE = 3;
const int PlayState::MAXIMUM_REFLECTION_ANGLE = ((3 * M_PI) / 12);
const int PlayState::POWERUP_PROBABILITY = 25;
const int PlayState::TOTAL_NUMBER_OF_LEVELS = 6;
const int PlayState::TOTAL_NUMBER_OF_POWERUPS = 8;
const int PlayState::TOTAL_NUMBER_OF_SOUNDS = 8;
const int PlayState::BONUS_POINTS_SHIELD = 50;
const int PlayState::BONUS_POINTS_LASER = 40;
const int PlayState::BONUS_POINTS_EXPANDED = 25;
const int PlayState::BONUS_POINTS_LIVES_MULTIPLIER = 12;
const int PlayState::BONUS_POINTS_LEVEL_MULTIPLIER = 15;
const int PlayState::MAX_LIVES = 5;
const int PlayState::POINTS_ALREADY_MAXED_LIVES = 50;
const int PlayState::POINTS_ALREADY_EXPANDED = 30;
const int PlayState::POINTS_SHIELD_ALREADY_ENABLED = 25;
const int PlayState::POINTS_LASER_ALREADY_ENABLED = 30;
const int PlayState::POINTS_PADDLE_ALREADY_SPED_UP = 20;
const int PlayState::POINTS_BALLS_ALREADY_SLOWED_DOWN = 20;
const int PlayState::START_LEVEL = 1;

PlayState PlayState::m_PlayState;

//using namespace Arkanoid;

PlayState::PlayState() :
    m_currentLevel(1),
    m_highScore(0),
    m_isSoundEnabled(true),
    m_isGameRunning(true),
    m_isBallLaunched(false)
    //lastFt{0.f},
    //currentSlice{0.f}
{
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

    m_player.Init(m_engine);
    brickGrid.Init(m_engine);
    hud.Init(m_engine);
    shield.Init(m_engine);

    LoadResources();
    LoadObjects();

    NewGame();
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
    //backgroundMusic.play();

    //timePoint1 = std::chrono::high_resolution_clock::now();

    sf::Event event;
    while (m_engine->getWindow().pollEvent(event)) {
        if (event.type == sf::Event::LostFocus) {
            SetGameRunning(false);
        } else if (event.type == sf::Event::GainedFocus) {
            SetGameRunning(true);
        }

        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S) {
            if (m_player.GetPaddle().HasLaser()) {
                projectiles.emplace_back(sf::Vector2f{m_player.GetPaddle().x() - (m_player.GetPaddle().GetSize().x / 2) + 8,
                    m_player.GetPaddle().y() - m_player.GetPaddle().GetSize().y});
                projectiles.emplace_back(sf::Vector2f{m_player.GetPaddle().x() + (m_player.GetPaddle().GetSize().x / 2) - 8,
                    m_player.GetPaddle().y() - m_player.GetPaddle().GetSize().y});

                if (IsSoundEnabled()) {
                    sounds.at(SoundEffect::FIRE_PROJECTILE).play();
                }
            }

        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G) {
            brickGrid.Clear();
        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
            brickGrid.MoveDown();
        }
    }

    if (!IsGameRunning()) {
        return;

    }

    // Handle player input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        m_engine->Quit();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        shield.SetEnabled(true);
        //m_engine->pushState(/*PausedState::Instance()*/new PausedState());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        if (balls.size() == 1) {
            if (!IsBallLaunched()) {
                SetBallLaunched(true);
                balls.front().SetVelocity({0, -balls.front().GetSpeed()});
            }
        }
    }

    if (!m_player.GetLives()) {
        NewGame();
    }

    if (brickGrid.IsEmpty()) {
        AddBonusPoints(GetLevel());
        LoadLevel(GetLevel() + 1);
    }

    if (balls.empty()) {
        RemovePowerups();

        if (!m_player.GainLives(-1)) {
            NewGame();
        }

        balls.emplace_back(m_engine, sf::Vector2f{m_player.GetPaddle().x(), m_player.GetPaddle().y() - ((m_player.GetPaddle().GetSize().y / 2) + balls.front().GetRadius())});
        SetBallLaunched(false);
        //if (isPlayerPlaying()) {
            if (IsSoundEnabled()) {
                sounds.at(SoundEffect::NEW_GAME).play();
            }
        //}
    }

    // Perform collision detection between all balls and paddle/shield/bricks
    for (auto& ball : balls) {
        TestCollision(m_player.GetPaddle(), ball);

        if (shield.IsEnabled()) {
            TestCollision(shield, ball);
        } else if (ball.y() > m_engine->getWindowSize().y) {
            ball.Destroy();
        }

        for (auto& brick : brickGrid.GetBricks()) {
            TestCollision(brick, ball);
        }
    }

    // Perform collision detection between paddle and all powerups
    for (auto& powerup : powerups) {
        TestCollision(m_player.GetPaddle(), powerup);
    }

    // Perform collision detection between projectiles and all bricks
    for (auto& projectile : projectiles) {
        for (auto& brick : brickGrid.GetBricks()) {
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
    if (!IsGameRunning()) {
        return;
    }

    if (IsBallLaunched()) {
        for (auto& ball : balls) {
            ball.Update();
        }
    } else {
        balls.front().SetPos({m_player.GetPaddle().x() - balls.front().GetRadius() / 2, m_player.GetPaddle().y() - ((balls.front().GetRadius() * 2) + 5)});
    }

    for (auto& powerup : powerups) {
        powerup.Update();
        //std::cout << "Updating powerup" << std::endl;
    }

    for (auto& projectile : projectiles) {
        projectile.Update(m_engine->getWindowSize());
        //std::cout << "Updating projectile" << std::endl;
    }

    /*currentSlice += lastFt;
    for(; currentSlice >= ftSlice; currentSlice -= ftSlice) {*/
        //ball.update(/*ftStep, */m_engine->getWindowSize());

        m_player.GetPaddle().Update(/*ftStep, */m_engine->getWindowSize());

        brickGrid.Update();

        // Remove destroyed balls from balls vector
        balls.erase(
            remove_if(begin(balls), end(balls),
                [](const Ball& mBall) {return mBall.IsDestroyed(); }),
            end(balls)
        );

        // Remove destroyed powerups from powerups vector
        powerups.erase(
            remove_if(begin(powerups), end(powerups),
                [](const Powerup& mPowerup) { return mPowerup.IsDestroyed(); }),
            end(powerups)
        );

        // Remove destroyed projectiles from projectiles vector
        projectiles.erase(
            remove_if(begin(projectiles), end(projectiles),
                [](const Projectile& mProjectile) { return mProjectile.IsDestroyed(); }),
            end(projectiles)
        );
    //}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        SetBallLaunched(true);
        balls.emplace_back(m_engine, sf::Vector2f{m_player.GetPaddle().x() - balls.back().GetRadius() / 2, m_player.GetPaddle().y() - 20});
        //balls.back().Init();
        balls.back().SetVelocity({0, -32});
    }
}

void PlayState::Draw()
{
    if (!IsGameRunning()) {
        return;
    }

    m_engine->getWindow().clear(BACKGROUND_CLEAR_COLOUR);

    // Draw background image
    m_engine->getWindow().draw(background.at(BackgroundObject::BACKGROUND));
    // Draw background shadows
    m_engine->getWindow().draw(background.at(BackgroundObject::SHADOW_TOP));
    m_engine->getWindow().draw(background.at(BackgroundObject::SHADOW_LEFT));

    // Draw bricks
    for (auto& brick : brickGrid.GetBricks()) {
        if (brick.IsVisible()) {
            brick.Draw();
        }
    }

    if (!IsBallLaunched()) {
        balls.front().SetPos({m_player.GetPaddle().x() - balls.front().GetRadius() / 2, m_player.GetPaddle().y() - ((balls.front().GetRadius() * 2) + 5)});
    }

    // Draw balls
    for (auto& ball : balls) {
        if (!ball.IsDestroyed()) {
            ball.Draw();
        }
    }

    // Draw powerups
    for (auto& powerup : powerups) {
        if (!powerup.IsDestroyed()) {
            powerup.Draw();
        }
    }

    // Draw projectiles
    for (auto& projectile : projectiles) {
        if (!projectile.IsDestroyed()) {
            projectile.Draw();
        }
    }

    // Draw paddle
    m_player.GetPaddle().Draw();

    // Draw shield
    if (shield.IsEnabled()) {
        shield.Draw();
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
    hud.displayHud(m_player.GetPoints(), GetHighScore(), m_player.GetLives(), GetLevel());

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
    SetLevel(START_LEVEL);
    SetHighScore(std::max(m_player.GetPoints(), GetHighScore()));
    m_player.Reset();

    brickGrid.GenerateGrid(GetLevel());

    powerups.clear();
    projectiles.clear();

    balls.clear();
    balls.emplace_back(m_engine, sf::Vector2f{m_engine->getWindowSize().x / 2, m_engine->getWindowSize().y - m_player.GetPaddle().GetPos().y - m_player.GetPaddle().GetSize().y});
    balls.front().SetPos({m_engine->getWindowSize().x, m_player.GetPaddle().top() - (balls.front().GetRadius() * 2) + 1});

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::NEW_GAME).play();
        backgroundMusic.play();
    }
}

void PlayState::RemovePowerups()
{
    shield.SetEnabled(false);
    m_player.GetPaddle().SetLaserEnabled(false);
}

void PlayState::AddBonusPoints(const int level)
{
    int bonusPoints = 0;

    if (shield.IsEnabled()) {
        bonusPoints += BONUS_POINTS_SHIELD;
    }

    if (m_player.GetPaddle().HasLaser()) {
        bonusPoints += BONUS_POINTS_LASER;
    }

    if (m_player.GetPaddle().IsExpanded()) {
        bonusPoints += BONUS_POINTS_EXPANDED;
    }

    bonusPoints += m_player.GetLives() * BONUS_POINTS_LIVES_MULTIPLIER;
    bonusPoints += level * BONUS_POINTS_LEVEL_MULTIPLIER;

    m_player.GainPoints(bonusPoints);
}

void PlayState::LoadLevel(const int level)
{
    if (level < 1) {
        return;
    }

    SetLevel((level < TOTAL_NUMBER_OF_LEVELS) ? level : 1);

    m_player.GetPaddle().Reset();
    powerups.clear();
    projectiles.clear();

    SetBallLaunched(false);
    balls.clear();
    balls.emplace_back(m_engine, sf::Vector2f{m_engine->getWindowSize().x / 2, m_engine->getWindowSize().y - m_player.GetPaddle().GetPos().y - m_player.GetPaddle().GetSize().y});
    balls.front().SetPos({m_engine->getWindowSize().x, m_player.GetPaddle().top() - (balls.front().GetRadius() * 2) + 1});

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::NEW_GAME).play();
        backgroundMusic.play();
    }

    brickGrid.GenerateGrid(level);
}

sf::Vector2f PlayState::CalculatePaddleReflectionVector(Paddle& mPaddle, Ball& mBall)
{
    // Calculate angle of reflection of the ball
    const float RELATIVE_INTERSECT_X{abs(mPaddle.x() - mBall.x()) - 4};
    const float NORMALISED_RELATIVE_INTERSECT_X{(RELATIVE_INTERSECT_X / (m_player.GetPaddle().GetSize().x / 2))};
    const float BOUNCE_ANGLE{NORMALISED_RELATIVE_INTERSECT_X * /*MAXIMUM_REFLECTION_ANGLE*/(3 * M_PI) / 12};

    // Calculate vector of ball reflecting/bouncing off of paddle
    sf::Vector2f newBallVelocity;
    newBallVelocity.x = mBall.GetSpeed() * sin(BOUNCE_ANGLE);
    newBallVelocity.y = -mBall.GetSpeed() * cos(BOUNCE_ANGLE);
    newBallVelocity.x = (mBall.x() < mPaddle.x()) ? -newBallVelocity.x : newBallVelocity.x;

    std::cout << "sin: " << sin(BOUNCE_ANGLE) << ", cos: " << cos(BOUNCE_ANGLE) << std::endl;

    return newBallVelocity;
}

sf::Vector2f PlayState::CalculateBrickReflectionVector(Brick& mBrick, Ball& mBall)
{
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

    // Calculate vector of ball reflecting/bouncing off of brick
    if (abs(minOverlapX) < abs(minOverlapY)) {
        newBallVelocity.x = -mBall.GetVelocity().x;
        newBallVelocity.y = (mBrick.y() < mBall.y()) ? abs(mBall.GetVelocity().y) : -abs(mBall.GetVelocity().y);
    } else {
        newBallVelocity.x = (mBall.x() < mBrick.x()) ? -mBall.GetVelocity().x : mBall.GetVelocity().x;
        newBallVelocity.y = ballFromTop ? -abs(mBall.GetVelocity().y) : abs(mBall.GetVelocity().y);
    }

    return newBallVelocity;
}

template <class T1, class T2>
bool PlayState::IsIntersecting(T1& mA, T2& mB)
{
    return mA.right() >= mB.left() && mA.left() <= mB.right()
        && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void PlayState::TestCollision(Paddle& mPaddle, Ball& mBall)
{
    if (!IsIntersecting(mPaddle, mBall)) {
        return;
    }

    mBall.SetVelocity(CalculatePaddleReflectionVector(mPaddle, mBall));

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::PADDLE_COLLISION).play();
    }
}

void PlayState::TestCollision(Shield& mShield, Ball& mBall)
{
    if (mBall.bottom() < mShield.top()) {
        return;
    }

    mBall.SetVelocity({mBall.GetVelocity().x, -mBall.GetVelocity().y});

    mShield.SetEnabled(false);

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::SHIELD_BOUNCE).play();
    }
}

void PlayState::TestCollision(Brick& mBrick, Ball& mBall)
{
    if (!IsIntersecting(mBrick, mBall)) {
        return;
    }

    if (mBrick.GetType() == 0) {
        mBrick.Destroy();
    }

    m_player.GainPoints(POINTS_BALL);

    mBall.SetVelocity(CalculateBrickReflectionVector(mBrick, mBall));

    if (mBrick.IsDestroyed()) {
        if (!((std::rand() % 100) % POWERUP_PROBABILITY)) {
            powerups.emplace_back(m_engine, sf::Vector2f{mBrick.x(), mBrick.y()});
        }
    }

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::BRICK_COLLISION).play();
    }
}

void PlayState::TestCollision(Brick
& mBrick, Projectile& mProjectile)
{
    if (!IsIntersecting(mBrick, mProjectile)) {
        return;
    }

    mBrick.Destroy();
    mProjectile.Destroy();

    m_player.GainPoints(POINTS_PROJECTILE);

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::BRICK_COLLISION).play();
    }
}

void PlayState::TestCollision(Paddle& mPaddle, Powerup& mPowerup)
{
    if (!IsIntersecting(mPaddle, mPowerup)) {
        return;
    }

    mPowerup.Destroy();

    if (IsSoundEnabled()) {
        sounds.at(SoundEffect::GAIN_POWERUP).play();
    }

    ApplyPowerup();
}

void PlayState::ApplyPowerup()
{
    const int POWERUP_NUMBER = std::rand() % TOTAL_NUMBER_OF_POWERUPS;

    switch (POWERUP_NUMBER) {
        // Player gains a life
        case 0:
            if (m_player.GetLives() < MAX_LIVES) {
                m_player.GainLives(1);
            } else {
                m_player.GainPoints(POINTS_ALREADY_MAXED_LIVES);
            }
            break;

        // Paddle expands
        case 1:
            if (!m_player.GetPaddle().IsExpanded()) {
                m_player.GetPaddle().Expand();
            } else {
                m_player.GainPoints(POINTS_ALREADY_EXPANDED);
            }
            break;

        // Balls slow down
        case 2:
            for (auto& ball : balls) {
                if (!ball.IsSlowedDown()) {
                    ball.SlowDown();
                } else {
                    //m_player.GainPoints(POINTS_BALLS_ALREADY_SLOWED_DOWN);
                }
            }
            break;

        // Player gets a new ball
        case 3:
            balls.emplace_back(m_engine, sf::Vector2f{m_player.GetPaddle().x() - balls.back().radius() / 2, m_player.GetPaddle().y() -20});
            break;

        // Player speeds up
        case 4:
            if (!m_player.GetPaddle().IsSpedUp()) {
                m_player.GetPaddle().SpeedUp();
            } else {
                m_player.GainPoints(POINTS_PADDLE_ALREADY_SPED_UP);
            }
            break;

        // Shield is enabled
        case 5:
            if (!shield.IsEnabled()) {
                shield.SetEnabled(true);
            } else {
                m_player.GainPoints(POINTS_SHIELD_ALREADY_ENABLED);
            }
            break;

        // Brick grid moves down
        case 6:
            brickGrid.MoveDown();

            if (IsSoundEnabled()) {
                sounds.at(SoundEffect::BRICKS_MOVE_DOWN).play();
            }
            break;

        // Player gets laser
        case 7:
            if (!m_player.GetPaddle().HasLaser()) {
                m_player.GetPaddle().SetLaserEnabled(true);
            } else {
                m_player.GainPoints(POINTS_LASER_ALREADY_ENABLED);
            }
            break;

        default:
            break;
    }
}

void PlayState::LoadResources()
{
    // Set up sounds vector
    for (int i{0}; i < TOTAL_NUMBER_OF_SOUNDS; i++) {
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
        background.back().setOrigin(0, 0);
    }

    LoadObjects();
}

void PlayState::LoadObjects()
{
    const int HUD_Y = hud.getScoreBackgroundHeight();
    const int WINDOW_X = m_engine->getWindowSize().x;
    const int WINDOW_Y = m_engine->getWindowSize().y;

    static const sf::Vector2f BORDER_CORNER_LEFT_SIZE = {20, 20};
    static const sf::Vector2f BORDER_CORNER_RIGHT_SIZE = {20, 20};

    // Set up left border corner shape
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setPosition(0, HUD_Y);
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setSize(BORDER_CORNER_LEFT_SIZE);
    background.at(BackgroundObject::BORDER_CORNER_LEFT).setTexture(&m_engine->resourceMan.GetTexture("border_corner_left.png"));

    const int CORNER_LEFT_X = background.at(BackgroundObject::BORDER_CORNER_LEFT).getSize().x;
    const int CORNER_LEFT_Y = background.at(BackgroundObject::BORDER_CORNER_LEFT).getSize().y;

    // Set up right border corner shape
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setPosition(WINDOW_X - CORNER_LEFT_X, 30);
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setSize(BORDER_CORNER_RIGHT_SIZE);
    background.at(BackgroundObject::BORDER_CORNER_RIGHT).setTexture(&m_engine->resourceMan.GetTexture("border_corner_right.png"));

    const int CORNER_RIGHT_X = background.at(BackgroundObject::BORDER_CORNER_RIGHT).getSize().x;
    const int CORNER_RIGHT_Y = background.at(BackgroundObject::BORDER_CORNER_RIGHT).getSize().y;

    // Set up top border shape
    background.at(BackgroundObject::BORDER_TOP).setPosition(CORNER_LEFT_X, HUD_Y);
    background.at(BackgroundObject::BORDER_TOP).setSize({WINDOW_X - CORNER_LEFT_X - CORNER_RIGHT_X, 20});
    background.at(BackgroundObject::BORDER_TOP).setTexture(&m_engine->resourceMan.GetTexture("border_top.png"));

    const int BORDER_TOP_Y = background.at(BackgroundObject::BORDER_TOP).getSize().y;

    // Set up left border shape
    background.at(BackgroundObject::BORDER_LEFT).setPosition(0, HUD_Y + CORNER_LEFT_Y);
    background.at(BackgroundObject::BORDER_LEFT).setSize({20, WINDOW_Y - HUD_Y - CORNER_LEFT_Y});
    background.at(BackgroundObject::BORDER_LEFT).setTexture(&m_engine->resourceMan.GetTexture("border_side.png"));

    // Set up right border shape
    background.at(BackgroundObject::BORDER_RIGHT).setPosition(WINDOW_X - CORNER_LEFT_X, HUD_Y + CORNER_RIGHT_Y);
    background.at(BackgroundObject::BORDER_RIGHT).setSize({20, WINDOW_Y - HUD_Y - CORNER_RIGHT_Y});
    background.at(BackgroundObject::BORDER_RIGHT).setTexture(&m_engine->resourceMan.GetTexture("border_side.png"));

    // Set up left shield shape
    background.at(BackgroundObject::SHIELD_LEFT).setPosition(0, WINDOW_Y - 50);
    background.at(BackgroundObject::SHIELD_LEFT).setSize({25, 20});
    background.at(BackgroundObject::SHIELD_LEFT).setTexture(&m_engine->resourceMan.GetTexture("shield_left.png"));

    // Set up right shield shape
    background.at(BackgroundObject::SHIELD_RIGHT).setPosition(WINDOW_X - 25, WINDOW_Y - 50);
    background.at(BackgroundObject::SHIELD_RIGHT).setSize({25, 20});
    background.at(BackgroundObject::SHIELD_RIGHT).setTexture(&m_engine->resourceMan.GetTexture("shield_right.png"));

    // Set up top shadow shape
    background.at(BackgroundObject::SHADOW_TOP).setPosition(CORNER_LEFT_X, HUD_Y + BORDER_TOP_Y);
    background.at(BackgroundObject::SHADOW_TOP).setSize({WINDOW_X - CORNER_LEFT_X - CORNER_RIGHT_X, 20});
    background.at(BackgroundObject::SHADOW_TOP).setFillColor({0, 0, 0, 127});

    // Set up left shadow shape
    background.at(BackgroundObject::SHADOW_LEFT).setPosition(20, HUD_Y + BORDER_TOP_Y + 20);
    background.at(BackgroundObject::SHADOW_LEFT).setSize({20, WINDOW_Y - HUD_Y - BORDER_TOP_Y});
    background.at(BackgroundObject::SHADOW_LEFT).setFillColor({0, 0, 0, 127});

    // Set up background shape
    background.at(BackgroundObject::BACKGROUND).setPosition(0, 0);
    background.at(BackgroundObject::BACKGROUND).setSize({WINDOW_X, WINDOW_Y});
    background.at(BackgroundObject::BACKGROUND).setTexture(&m_engine->resourceMan.GetTexture("hexagon_pattern.png"));
}
