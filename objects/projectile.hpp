#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "..\stateman\gameEngine.hpp"
#include "rectangle.hpp"

class Projectile : public Rectangle
{
public:
    Projectile(GameEngine *game, const sf::Vector2f position);
    ~Projectile();

    void Update(/*const float mFT*/);

    void Draw();

    bool IsDestroyed() const noexcept { return m_isDestroyed; }
    void Destroy() noexcept { m_isDestroyed = true; }

    sf::Texture texture;

private:
    GameEngine *m_engine;

    sf::Vector2f velocity;
    bool m_isDestroyed;
    int m_projectileVelocity;

    static const sf::Vector2f SIZE;
    static const int DEFAULT_SPEED;
};

#endif // PROJECTILE_HPP
