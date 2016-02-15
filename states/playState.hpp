#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include <SFML\Audio.hpp>
//#include <chrono>

#include "gameState.hpp"
#include "..\hud\hud.hpp"
#include "..\objects\ball.hpp"
#include "..\objects\paddle.hpp"
#include "..\objects\brick.hpp"
#include "..\objects\brickGrid.hpp"
#include "..\objects\powerup.hpp"
#include "..\objects\projectile.hpp"
#include "..\objects\shield.hpp"
//#include "..\levels\levels.hpp"
//#include "..\objects\powerupExtraLife.hpp"
//#include "pausedState.hpp"

/*namespace Arkanoid
{*/
    class PlayState : public GameState
    {
    public:
        PlayState();
        ~PlayState();

        void Init(GameEngine *game);
        void Cleanup();

        void Pause();
        void Resume();

        void HandleEvents();
        void Update();
        void Draw();

        void LoadResources();
        void LoadObjects();

        void NewGame();
        void LoadLevel(const int level);
        void GenerateNewBrickGrid(const int level);

        bool IsGameRunning() const noexcept { return m_isGameRunning; }
        void SetGameRunning(const bool state) noexcept { m_isGameRunning = state; }

        void ApplyPowerup();
        void RemovePowerups();
        void AddBonusPoints(const int level);

        //int GainLives(const int numLives) { return playerLives += numLives; }

        inline bool IsBallLaunched() const noexcept { return m_isBallLaunched; }
        inline void LaunchBall() noexcept { m_isBallLaunched = true; }
        inline void DockBall() noexcept { m_isBallLaunched = false; }

        inline bool IsSoundEnabled() const noexcept { return m_isSoundEnabled; }
        inline void SetSoundEnabled(const bool state) noexcept { m_isSoundEnabled = state; }

        template <class T1, class T2>
        bool IsIntersecting(T1& mA, T2& mB);
        void TestCollision(Paddle& mPaddle, Ball& mBall);
        void TestCollision(Brick& mBrick, Ball& mBall);
        void TestCollision(Paddle& mPaddle, Powerup& mPowerup);
        void TestCollision(Brick& mBrick, Projectile& mProjectile);
        void TestCollision(Shield& mShield, Ball& mBall);

        sf::Vector2f CalculatePaddleReflectionVector(Paddle& mPaddle, Ball& mBall);
        sf::Vector2f CalculateBrickReflectionVector(Brick& mBrick, Ball& mBall);

        static PlayState* Instance() {
            return &m_PlayState;
        }

        //std::chrono::high_resolution_clock::timePoint1;
        //std::chrono::high_resolution_clock::timePoint2;

    private:
        enum /*class*/ SoundEffect : int {
            BRICK_COLLISION = 0,
            PADDLE_COLLISION = 1,
            NEW_GAME = 2,
            LOSE_LIFE = 3,
            GAIN_POWERUP = 4,
            FIRE_PROJECTILE = 5,
            SHIELD_BOUNCE = 6,
            BRICKS_MOVE_DOWN = 7
        };

        enum /*class*/ BackgroundObject : int {
            BORDER_TOP = 0,
            BORDER_LEFT = 1,
            BORDER_RIGHT = 2,
            BORDER_CORNER_LEFT = 3,
            BORDER_CORNER_RIGHT = 4,
            SHIELD_LEFT = 5,
            SHIELD_RIGHT = 6,
            SHADOW_TOP = 7,
            SHADOW_LEFT = 8,
            BACKGROUND = 9
        };

        static PlayState m_PlayState;
        GameEngine *m_engine;

        std::vector<sf::RectangleShape> background;
        std::vector<sf::Sound> sounds;
        sf::Music backgroundMusic;

        Paddle paddle;
        Shield shield;
        Hud hud;
        std::vector<Ball> balls;
        //std::vector<Brick> bricks;
        std::vector<Powerup> powerups;
        std::vector<Projectile> projectiles;
        BrickGrid brickGrid;

        bool m_isPlayerPlaying;
        bool m_isSoundEnabled;
        bool m_isGameRunning;
        bool m_isBallLaunched;

        int playerLives;
        int playerLevel;
        int playerScore;
        int highScore;

        /*float lastFt;
        float currentSlice;
        static const float ftStep;
        static const float ftSlice;*/

        bool keyAlreadyPressed{false};

        static const sf::Vector2f initBallPos;
        static const float ballRadius;
        static const sf::Color ballColour;

        static const sf::Vector2f BRICK_SIZE;
        static const int POINTS_BALL;
        static const int POINTS_PROJECTILE;
        static const int MAXIMUM_REFLECTION_ANGLE;
        static const int DEFAULT_PLAYER_LIVES;
        static const int POWERUP_PROBABILITY;
        static const int TOTAL_NUMBER_OF_POWERUPS;        
    };
//}

#endif // PLAY_STATE_HPP
