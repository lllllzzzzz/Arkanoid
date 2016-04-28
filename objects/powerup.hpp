#ifndef POWERUP_HPP
#define POWERUP_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"

class Powerup : public Rectangle
{
public:
    Powerup(float posX, float posY, int width, int height, int speed, sf::Color color, sf::Texture& texture);
    ~Powerup();

    void update(/*const float mFT, */const int windowWidth, const int windowHeight);

    bool isDestroyed() const noexcept { return m_isDestroyed; }
    void destroy() { m_isDestroyed = true; }

private:
    sf::Vector2f velocity;
    bool m_isDestroyed;
    int m_powerupVelocity;
};

#endif // POWERUP_HPP
