#include "circle.hpp"

Circle::Circle()
{

}

Circle::~Circle()
{

}

float Circle::x() const noexcept
{
    return shape.getPosition().x;
}

float Circle::y() const noexcept
{
    return shape.getPosition().y;
}

float Circle::left() const noexcept
{
    return x() - shape.getRadius();
}

float Circle::right() const noexcept
{
    return x() + shape.getRadius();
}

float Circle::top() const noexcept
{
    return y() - shape.getRadius();
}

float Circle::bottom() const noexcept
{
    return y() + shape.getRadius();
}

float Circle::radius() const noexcept
{
    return shape.getRadius();
}