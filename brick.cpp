#include "Brick.hpp"

Brick::Brick(float posX, float posY)
{
    shape.setPosition(posX, posY);
    shape.setSize({brickWidth, brickHeight});
    shape.setFillColor(Color::Red);
    shape.setOrigin(brickWidth / 2.f, brickHeight / 2.f);
    isDestroyed = false;
}

Brick::~Brick()
{

}

float Brick::x()
{
    return shape.getPosition().x;
}

float Brick::y()
{
    return shape.getPosition().y;
}

float Brick::left()
{
    return x() - shape.getSize().x / 2.f;
}

float Brick::right()
{
    return x() + shape.getSize().x / 2.f;
}

float Brick::top()
{
    return y() - shape.getSize().y / 2.f;
}

float Brick::bottom()
{
    return y() + shape.getSize().y / 2.f;
}
