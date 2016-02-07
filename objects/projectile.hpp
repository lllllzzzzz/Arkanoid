#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"

class Projectile : public Rectangle
{
public:
    Projectile(float posX, float posY);
    ~Projectile();

    void update(/*const float mFT, */const int windowWidth, const int windowHeight);

    bool isDestroyed() const noexcept { return m_isDestroyed; }
    void destroy() { m_isDestroyed = true; }

    sf::Texture texture;

private:
    sf::Vector2f velocity;
    bool m_isDestroyed;
    int m_projectileVelocity;

    static const int m_width;
    static const int m_height;
    static const int m_speedDefault;
};

#endif // PROJECTILE_HPP
