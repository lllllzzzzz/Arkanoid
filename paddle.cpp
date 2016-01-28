#include "Paddle.hpp"

#define windowWidth 750
#define windowHeight 450

Paddle::Paddle(float posX, float posY, int w, int h)
{
    shape.setPosition(posX, posY);
    shape.setSize({w, h});
    shape.setFillColor(Color::White);
    shape.setOrigin(w / 2, h / 2);
}

Paddle::~Paddle()
{

}

void Paddle::update()
{
    shape.move(velocity);

    // Handle player input (move paddle left or right)
    if (Keyboard::isKeyPressed(Keyboard::Key::A) && left() > 0) {
        velocity.x = -paddleVelocity;
    } else if (Keyboard::isKeyPressed(Keyboard::Key::D)
        && right() < windowWidth) {
        velocity.x = paddleVelocity;
    } else {
        velocity.x = 0;
    }
}

float Paddle::x()
{
    return shape.getPosition().x;
}

float Paddle::y()
{
    return shape.getPosition().y;
}

float Paddle::left()
{
    return x() - shape.getSize().x / 2.f;
}

float Paddle::right()
{
    return x() + shape.getSize().x / 2.f;
}

float Paddle::top()
{
    return y() - shape.getSize().y / 2.f;
}

float Paddle::bottom()
{
    return y() + shape.getSize().y / 2.f;
}

void Paddle::setPos(int posX, int posY)
{
    shape.setPosition(posX, posY);
}
