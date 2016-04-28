#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <SFML\Graphics.hpp>

class Rectangle
{
public:
    Rectangle();
    ~Rectangle();

    virtual float x() const noexcept; //{ return shape.getPosition().x; }
    virtual float y() const noexcept; //{ return shape.getPosition().y; }
    virtual float left() const noexcept; //{ return x() - shape.getSize().x / 2.f; }
    virtual float right() const noexcept; //{ return x() + shape.getSize().x / 2.f; }
    virtual float top() const noexcept; //{ return y() - shape.getSize().y / 2.f; }
    virtual float bottom() const noexcept; //{ return y() + shape.getSize().y / 2.f; }

    void setPos(const int x, const int y); //{ shape.setPosition(x, y); }

    sf::RectangleShape shape;
};

#endif // RECTANGLE_HPP