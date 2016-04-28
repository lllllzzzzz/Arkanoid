#ifndef BALL_HPP
#define BALL_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "circle.hpp"
#include "..\stateman\gameEngine.hpp"

class Ball : public Circle
{
public:
    Ball(GameEngine *game, const sf::Vector2f position);
    ~Ball();

    void Init(GameEngine *game);

    void Draw();

    void Update(/*const float mFT, */);
    void SetPos(const sf::Vector2f newPosition) noexcept;
    sf::Vector2f GetPos() const noexcept;
    int GetRadius() const noexcept;
    bool IsDestroyed() const noexcept;

    void SetVelocity(const sf::Vector2f newVelocity) noexcept;
    sf::Vector2f GetVelocity() const noexcept { return m_velocity; }

    void SetSpeed(const int speed) noexcept { m_speed = speed; }
    int GetSpeed() const noexcept { return m_speed; }

    void SlowDown() noexcept;
    void SpeedUp() noexcept;
    bool IsSpedUp() const noexcept { return m_speed < DEFAULT_SPEED; }
    bool IsSlowedDown() const noexcept { return m_speed > DEFAULT_SPEED; }
    
    void Destroy() noexcept;

    sf::CircleShape shadow;

private:
    GameEngine *m_engine;

    sf::Vector2f m_velocity;

    int m_speed;
    int m_radius;
    bool m_isDestroyed;

    sf::Sound m_hitBordersSound;

    static const sf::Color DEFAULT_COLOUR;
    static const int DEFAULT_SPEED;
    static const int DEFAULT_RADIUS;
    static const int SHADOW_OPACITY;
    static const int SLOW_DOWN_FACTOR;
    static const int SPEED_UP_FACTOR;
};

#endif // BALL_HPP
