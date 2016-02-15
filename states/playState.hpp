#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include <SFML\Audio.hpp>
//#include <chrono>

#include "gameState.hpp"
#include "..\hud\hud.hpp"
#include "..\objects\ball.hpp"
#include "..\objects\paddle.hpp"
#include "..\objects\brick.hpp"
//#include "..\objects\brickGrid.hpp"
#include "..\objects\powerup.hpp"
#include "..\objects\projectile.hpp"
#include "..\objects\shield.hpp"
#include "..\levels\levels.hpp"
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
        void PauseGame() { m_isGameRunning = false; }
        void ResumeGame() { m_isGameRunning = true; }

        void ApplyPowerup();
        void RemovePowerups();
        void AddBonusPoints(const int level);

        //int GainLives(const int numLives) { return playerLives += numLives; }

        inline bool IsBallLaunched() const noexcept { return m_isBallLaunched; }
        inline void LaunchBall() { m_isBallLaunched = true; }
        inline void DockBall() { m_isBallLaunched = false; }

        inline bool IsSoundEnabled() const noexcept { return m_isSoundEnabled; }
        inline void EnableSound() { m_isSoundEnabled = true; }
        inline void DisableSound() { m_isSoundEnabled = false; }

        template <class T1, class T2>
        bool IsIntersecting(T1& mA, T2& mB);
        void TestCollision(Paddle& mPaddle, Ball& mBall);
        void TestCollision(Brick& mBrick, Ball& mBall);
        void TestCollision(Paddle& mPaddle, Powerup& mPowerup);
        void TestCollision(Brick& mBrick, Projectile& mProjectile);
        void TestCollision(Shield& mShield, Ball& mBall);

        static PlayState* Instance() {
            return &m_PlayState;
        }

        //std::chrono::high_resolution_clock::timePoint1;
        //std::chrono::high_resolution_clock::timePoint2;

    private:
        template <typename T>
        struct brickData {
            T x;
            T y;
            sf::Color colour;
        };

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
        std::vector<Brick> bricks;
        std::vector<Powerup> powerups;
        std::vector<Projectile> projectiles;
        //BrickGrid brickGrid;

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
        static const int DEFAULT_PLAYER_LIVES;
        static const int POWERUP_PROBABILITY;
        static const int TOTAL_NUMBER_OF_POWERUPS;

        std::vector<brickData<double>> currentLevel;

        std::vector<brickData<double>> level1 = {
            {1, 0, sf::Color::Green},
            {2, 0, sf::Color::Green},
            {3, 0, sf::Color::Green},
            {4, 0, sf::Color::Green},
            {5, 0, sf::Color::Green},
            {6, 0, sf::Color::Green},
            {7, 0, sf::Color::Green},
            {8, 0, sf::Color::Green},
            {9, 0, sf::Color::Green},

            {1, 1, sf::Color::Green},
            {2, 1, sf::Color::Green},
            {3, 1, sf::Color::Green},
            {4, 1, sf::Color::Green},
            {5, 1, sf::Color::Green},
            {6, 1, sf::Color::Green},
            {7, 1, sf::Color::Green},
            {8, 1, sf::Color::Green},
            {9, 1, sf::Color::Green},

            {1, 2, sf::Color::Blue},
            {2, 2, sf::Color::Blue},
            {3, 2, sf::Color::Blue},
            {4, 2, sf::Color::Blue},
            {5, 2, sf::Color::Blue},
            {6, 2, sf::Color::Blue},
            {7, 2, sf::Color::Blue},
            {8, 2, sf::Color::Blue},
            {9, 2, sf::Color::Blue},

            {1, 3, sf::Color::Blue},
            {2, 3, sf::Color::Blue},
            {3, 3, sf::Color::Blue},
            {4, 3, sf::Color::Blue},
            {5, 3, sf::Color::Blue},
            {6, 3, sf::Color::Blue},
            {7, 3, sf::Color::Blue},
            {8, 3, sf::Color::Blue},
            {9, 3, sf::Color::Blue},

            {1, 4, sf::Color::Red},
            {2, 4, sf::Color::Red},
            {3, 4, sf::Color::Red},
            {4, 4, sf::Color::Red},
            {5, 4, sf::Color::Red},
            {6, 4, sf::Color::Red},
            {7, 4, sf::Color::Red},
            {8, 4, sf::Color::Red},
            {9, 4, sf::Color::Red},

            {1, 5, sf::Color::Red},
            {2, 5, sf::Color::Red},
            {3, 5, sf::Color::Red},
            {4, 5, sf::Color::Red},
            {5, 5, sf::Color::Red},
            {6, 5, sf::Color::Red},
            {7, 5, sf::Color::Red},
            {8, 5, sf::Color::Red},
            {9, 5, sf::Color::Red},
        };

        std::vector<brickData<double>> level2 = {
            {0, 0, sf::Color::Blue},
            {1, 0, sf::Color::Blue},
            {2, 0, sf::Color::Blue},
            {3, 0, sf::Color::Blue},
            {4, 0, sf::Color::Blue},
            {5, 0, sf::Color::Blue},
            {6, 0, sf::Color::Blue},
            {7, 0, sf::Color::Blue},
            {8, 0, sf::Color::Blue},
            {9, 0, sf::Color::Blue},
            {10, 0, sf::Color::Blue},

            {1, 1, sf::Color::Magenta},
            {2, 1, sf::Color::Magenta},
            {3, 1, sf::Color::Magenta},
            {4, 1, sf::Color::Magenta},
            {5, 1, sf::Color::Magenta},
            {6, 1, sf::Color::Magenta},
            {7, 1, sf::Color::Magenta},
            {8, 1, sf::Color::Magenta},
            {9, 1, sf::Color::Magenta},

            {2, 2, sf::Color::Blue},
            {3, 2, sf::Color::Blue},
            {4, 2, sf::Color::Blue},
            {5, 2, sf::Color::Blue},
            {6, 2, sf::Color::Blue},
            {7, 2, sf::Color::Blue},
            {8, 2, sf::Color::Blue},

            {3, 3, sf::Color::Cyan},
            {4, 3, sf::Color::Cyan},
            {5, 3, sf::Color::Cyan},
            {6, 3, sf::Color::Cyan},
            {7, 3, sf::Color::Cyan},

            {4, 4, sf::Color::White},
            {5, 4, sf::Color::White},
            {6, 4, sf::Color::White},

            {5, 5, sf::Color::Red},
        };

        std::vector<brickData<double>> level3 = {
            {1, 0, sf::Color::Green},
            {2, 0, sf::Color::Green},
            {3, 0, sf::Color::Green},
            {4, 0, sf::Color::Green},
            {5, 0, sf::Color::Green},
            {6, 0, sf::Color::Green},
            {7, 0, sf::Color::Green},
            {8, 0, sf::Color::Green},
            {9, 0, sf::Color::Green},

            {1, 1, sf::Color::Green},
            {9, 1, sf::Color::Green},

            {1, 2, sf::Color::Blue},
            {3, 2, sf::Color::Blue},
            {4, 2, sf::Color::Blue},
            {5, 2, sf::Color::Blue},
            {6, 2, sf::Color::Blue},
            {7, 2, sf::Color::Blue},
            {9, 2, sf::Color::Blue},

            {1, 3, sf::Color::Blue},
            {3, 3, sf::Color::Blue},
            {4, 3, sf::Color::Blue},
            {5, 3, sf::Color::Blue},
            {6, 3, sf::Color::Blue},
            {7, 3, sf::Color::Blue},
            {9, 3, sf::Color::Blue},

            {1, 4, sf::Color::Red},
            {9, 4, sf::Color::Red},

            {1, 5, sf::Color::Red},
            {9, 5, sf::Color::Red},

            {1, 5, sf::Color::Green},
            {9, 5, sf::Color::Green},

            {1, 5, sf::Color::Cyan},
            {2, 5, sf::Color::Cyan},
            {3, 5, sf::Color::Cyan},
            {4, 5, sf::Color::Cyan},
            {5, 5, sf::Color::Cyan},
            {6, 5, sf::Color::Cyan},
            {7, 5, sf::Color::Cyan},
            {8, 5, sf::Color::Cyan},
            {9, 5, sf::Color::Cyan},
        };

        std::vector<brickData<double>> level4 = {
            {0, 0, sf::Color::Red},
            {1, 0, sf::Color::Red},
            {3, 0, sf::Color::Red},
            {4, 0, sf::Color::Red},
            {6, 0, sf::Color::Red},
            {7, 0, sf::Color::Red},
            {9, 0, sf::Color::Red},
            {10, 0, sf::Color::Red},

            {5, 2, sf::Color::Red},

            {4, 3, sf::Color::White},
            {5, 3, sf::Color::White},
            {6, 3, sf::Color::White},

            {3, 4, sf::Color::Cyan},
            {4, 4, sf::Color::Cyan},
            {5, 4, sf::Color::Cyan},
            {6, 4, sf::Color::Cyan},
            {7, 4, sf::Color::Cyan},

            {2, 5, sf::Color::Blue},
            {3, 5, sf::Color::Blue},
            {4, 5, sf::Color::Blue},
            {5, 5, sf::Color::Blue},
            {6, 5, sf::Color::Blue},
            {7, 5, sf::Color::Blue},
            {8, 5, sf::Color::Blue},

            {1, 6, sf::Color::Magenta},
            {2, 6, sf::Color::Magenta},
            {3, 6, sf::Color::Magenta},
            {4, 6, sf::Color::Magenta},
            {5, 6, sf::Color::Magenta},
            {6, 6, sf::Color::Magenta},
            {7, 6, sf::Color::Magenta},
            {8, 6, sf::Color::Magenta},
            {9, 6, sf::Color::Magenta},

            {0, 7, sf::Color::Blue},
            {1, 7, sf::Color::Blue},
            {2, 7, sf::Color::Blue},
            {3, 7, sf::Color::Blue},
            {4, 7, sf::Color::Blue},
            {5, 7, sf::Color::Blue},
            {6, 7, sf::Color::Blue},
            {7, 7, sf::Color::Blue},
            {8, 7, sf::Color::Blue},
            {9, 7, sf::Color::Blue},
            {10, 7, sf::Color::Blue},
        };

        std::vector<brickData<double>> level5 = {
            {1, 0, sf::Color::Blue},
            {2, 0, sf::Color::Blue},
            {3, 0, sf::Color::Blue},
            {4, 0, sf::Color::Blue},
            {5, 0, sf::Color::Blue},
            {6, 0, sf::Color::Blue},
            {7, 0, sf::Color::Blue},
            {8, 0, sf::Color::Blue},
            {9, 0, sf::Color::Blue},

            {2, 1, sf::Color::Green},
            {3, 1, sf::Color::Green},
            {4, 1, sf::Color::Green},
            {5, 1, sf::Color::Green},
            {6, 1, sf::Color::Green},
            {7, 1, sf::Color::Green},
            {8, 1, sf::Color::Green},

            {3, 2, sf::Color::Cyan},
            {4, 2, sf::Color::Cyan},
            {5, 2, sf::Color::Cyan},
            {6, 2, sf::Color::Cyan},
            {7, 2, sf::Color::Cyan},

            {4, 3, sf::Color::Red},
            {5, 3, sf::Color::Red},
            {6, 3, sf::Color::Red},

            {3, 4, sf::Color::Cyan},
            {4, 4, sf::Color::Cyan},
            {5, 4, sf::Color::Cyan},
            {6, 4, sf::Color::Cyan},
            {7, 4, sf::Color::Cyan},

            {2, 5, sf::Color::Green},
            {3, 5, sf::Color::Green},
            {4, 5, sf::Color::Green},
            {5, 5, sf::Color::Green},
            {6, 5, sf::Color::Green},
            {7, 5, sf::Color::Green},
            {8, 5, sf::Color::Green},

            {1, 6, sf::Color::Blue},
            {2, 6, sf::Color::Blue},
            {3, 6, sf::Color::Blue},
            {4, 6, sf::Color::Blue},
            {5, 6, sf::Color::Blue},
            {6, 6, sf::Color::Blue},
            {7, 6, sf::Color::Blue},
            {8, 6, sf::Color::Blue},
            {9, 6, sf::Color::Blue},
        };

        std::vector<std::vector<brickData<double>>> levels;/* = {
            level1, level2, level3, level4, level5
        };*/
    };
//}

#endif // PLAY_STATE_HPP
