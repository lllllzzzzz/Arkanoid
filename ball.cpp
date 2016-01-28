#include "ball.hpp"

Ball::Ball(float posX, float posY, int radius)
{
    shape.setPosition(posX, posY);
    shape.setRadius(radius);
    shape.setFillColor(Color::White);
    shape.setOrigin(radius / 2, radius / 2);
}

Ball::~Ball()
{

}

float Ball::x() const noexcept
{
    return shape.getPosition().x;
}

float Ball::y() const noexcept
{
    return shape.getPosition().y;
}

float Ball::left() const noexcept
{
    return x() - shape.getRadius();
}

float Ball::right() const noexcept
{
    return x() + shape.getRadius();
}

float Ball::top() const noexcept
{
    return y() - shape.getRadius();
}

float Ball::bottom() const noexcept
{
    return y() + shape.getRadius();
}

void Ball::setPos(int posX, int posY)
{
    shape.setPosition(posX, posY);
}

void Ball::setVelocity(int x, int y)
{
    velocity.x = x;
    velocity.y = y;
}

void Ball::update()
{
    shape.move(velocity);

    // Keep the ball in horizontal bounds of the window
    if (left() < 0) {
        velocity.x = ballVelocity;
    } else if (right() > windowWidth) {
        velocity.x = -ballVelocity;
    }
    // Keep the ball in vertical bounds of the window
    if (top() < 0) {
        velocity.y = ballVelocity;
    } else if (bottom() > windowHeight) {
        velocity.y = -ballVelocity;
    }
}
