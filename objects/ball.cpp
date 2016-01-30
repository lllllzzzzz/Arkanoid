#include "ball.hpp"

Ball::Ball(const float posX, const float posY, const float radius, sf::Color color)
{
    shape.setPosition(posX, posY);
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setOrigin(radius / 2, radius / 2);
}

Ball::~Ball()
{

}

void Ball::setPos(const float posX, const float posY)
{
    shape.setPosition(posX, posY);
}

void Ball::setVelocity(const int x, const int y)
{
    velocity.x = x;
    velocity.y = y;
}

void Ball::update(const int windowWidth, const int windowHeight)
{
    // Move ball at velocity and angle specified by vector
    shape.move(velocity);

    // Rebound ball off of sides of window
    if (left() < 0) {
        velocity.x = ballVelocity;
    } else if (right() > windowWidth) {
        velocity.x = -ballVelocity;
    }

    // Rebound ball off of top of window
    if (top() < 0) {
        velocity.y = ballVelocity;
    }
}
