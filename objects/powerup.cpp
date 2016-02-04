#include "powerup.hpp"

Powerup::Powerup(float posX, float posY, int width, int height, int speed, sf::Color color, sf::Texture& texture) :
m_powerupVelocity(speed),
velocity(0, speed),
m_isDestroyed(false)
{
    shape.setPosition(posX, posY);
    shape.setSize({width, height});
    //shape.setFillColor(color);
    shape.setOrigin(width / 2.f, height / 2.f);
    shape.setTexture(&texture);
}

Powerup::~Powerup()
{

}

void Powerup::update(const int windowWidth, const int windowHeight)
{
    // Move ball at velocity and angle specified by vector
    shape.move(velocity/* * mFT*/);

    if (y() >= windowHeight) {
        destroy();
    }
}

/*bool Powerup::isDestroyed() const noexcept
{
    return m_isDestroyed;
}

void Powerup::destroy()
{
    m_isDestroyed = true;
}*/
