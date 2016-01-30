#include "Paddle.hpp"

#define windowWidth 750
#define windowHeight 450

Paddle::Paddle(float posX, float posY, int width, int height, sf::Color color)
{
    shape.setPosition(posX, posY);
    shape.setSize({width, height});
    shape.setFillColor(color);
    shape.setOrigin(width / 2, height / 2);
}

Paddle::~Paddle()
{

}

void Paddle::update()
{
    shape.move(velocity);

    // Handle player input (move paddle left or right)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && left() > 0) {
        velocity.x = -paddleVelocity;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
        && right() < windowWidth) {
        velocity.x = paddleVelocity;
    } else {
        velocity.x = 0;
    }
}

void Paddle::setPos(const int posX, const int posY)
{
    shape.setPosition(posX, posY);
}
