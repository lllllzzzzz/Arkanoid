#ifndef PADDLE_HPP
#define PADDLE_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "..\stateman\gameEngine.hpp"
#include "rectangle.hpp"

class Paddle : public Rectangle
{
public:
    Paddle(const sf::Vector2f position = {480 / 2, 620 - 40}, const sf::Vector2f size = {80, 15}, sf::Color color = sf::Color(48, 48, 48));
    ~Paddle();

    void Init(GameEngine *game);

    void Draw();

    void Reset();

    void Update(/*const float mFT, */ const sf::Vector2f windowSize);
    void SetPos(const sf::Vector2f position) noexcept;
    sf::Vector2f GetPos() const noexcept { return {x(), y()}; }
    void SetSize(const sf::Vector2f size);
    sf::Vector2f GetSize() const noexcept { return m_size; }
    void SetSpeed(const int speed) noexcept;
    int GetSpeed() const noexcept;

    bool HasLaser() const noexcept { return m_hasLaser; }
    void SetLaserEnabled(const bool state) noexcept { m_hasLaser = state; }

    bool IsExpanded() const noexcept { return GetSize().x > DEFAULT_SIZE.x; }
    bool IsShrunk() const noexcept { return GetSize().x < DEFAULT_SIZE.x; }

    void SpeedUp() noexcept;
    void SlowDown() noexcept;
    bool IsSpedUp() const noexcept { return m_speed < DEFAULT_SPEED; }
    bool IsSlowedDown() const noexcept { return m_speed > DEFAULT_SPEED; }

    void Expand() noexcept;
    void Shrink() noexcept;

    sf::RectangleShape shadow;

private:
    GameEngine *m_engine;

    sf::Vector2f velocity;

    sf::Vector2f m_size;
    int m_speed;
    int m_newWidth;

    bool m_hasLaser;

    static const float DEFAULT_SPEED;
    static const sf::Vector2f DEFAULT_SIZE;
    static const int ELEVATION;
    static const int SHADOW_OPACITY;
    static const int SPEED_UP_FACTOR;
    static const int SLOW_DOWN_FACTOR;
    static const int EXPAND_FACTOR;
    static const int SHRINK_FACTOR;
};

#endif // PADDLE_HPP
