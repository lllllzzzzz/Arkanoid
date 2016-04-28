#include "projectile.hpp"

const sf::Vector2f Projectile::SIZE = {5, 15};
const int Projectile::DEFAULT_SPEED = 12;

Projectile::Projectile(GameEngine *game, const sf::Vector2f position) :
m_projectileVelocity(DEFAULT_SPEED),
velocity(0, -DEFAULT_SPEED),
m_isDestroyed(false)
{
    m_engine = game;

    shape.setPosition(position);
    shape.setSize(SIZE);
    shape.setFillColor({255, 255, 255});
    shape.setOrigin(SIZE.x / 2.f, SIZE.y / 2.f);
    //shape.setTexture(&texture);
}

Projectile::~Projectile()
{

}

void Projectile::Update()
{
    shape.move(velocity/* * mFT*/);

    if (y() <= 30 + 20) {
        Destroy();
    }
}

void Projectile::Draw()
{
    m_engine->getWindow().draw(shape);
}
