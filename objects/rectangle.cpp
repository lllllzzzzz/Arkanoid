#include "rectangle.hpp"

Rectangle::Rectangle()
{

}

Rectangle::~Rectangle()
{

}

float Rectangle::x() const noexcept
{
    return shape.getPosition().x;
}

float Rectangle::y() const noexcept
{
    return shape.getPosition().y;
}

float Rectangle::left() const noexcept
{
    return x() - shape.getSize().x / 2.f;
}

float Rectangle::right() const noexcept
{
    return x() + shape.getSize().x / 2.f;
}

float Rectangle::top() const noexcept
{
    return y() - shape.getSize().y / 2.f;
}

float Rectangle::bottom() const noexcept
{
    return y() + shape.getSize().y / 2.f;
}

void Rectangle::setPos(const int x, const int y)
{
    shape.setPosition(x, y);
}