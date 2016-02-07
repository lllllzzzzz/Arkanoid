#include "projectile.hpp"

const int Projectile::m_width = 5;
const int Projectile::m_height = 15;
const int Projectile::m_speedDefault = 12;

Projectile::Projectile(float posX, float posY) :
m_projectileVelocity(m_speedDefault),
velocity(0, -m_speedDefault),
m_isDestroyed(false)
{
    shape.setPosition(posX, posY);
    shape.setSize({m_width, m_height});
    shape.setFillColor({255, 255, 255});
    shape.setOrigin(m_width / 2.f, m_height / 2.f);
    //shape.setTexture(&texture);
}

Projectile::~Projectile()
{

}

void Projectile::update(const int windowWidth, const int windowHeight)
{
    // Move projectile at velocity and angle specified by vector
    shape.move(velocity/* * mFT*/);

    if (y() <= 0) {
        destroy();
    }
}
