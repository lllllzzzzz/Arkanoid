#ifndef POWERUP_HPP
#define POWERUP_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "..\stateman\gameEngine.hpp"
#include "rectangle.hpp"

class Powerup : public Rectangle
{
public:
    Powerup(GameEngine *game, const sf::Vector2f position);
    ~Powerup();

    void Update(/*const float mFT, */);

    void Draw();

    bool IsDestroyed() const noexcept { return m_isDestroyed; }
    void Destroy() noexcept { m_isDestroyed = true; }

    sf::RectangleShape shadow;

private:
    GameEngine *m_engine;

    sf::Vector2f velocity;
    bool m_isDestroyed;
    int m_speed;

    int m_width;
    int m_height;

    static const int DEFAULT_SPEED;
    static const sf::Vector2f DEFAULT_SIZE;
};

#endif // POWERUP_HPP