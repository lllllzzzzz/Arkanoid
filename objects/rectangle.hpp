#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <SFML\Graphics.hpp>

class Rectangle
{
public:
    Rectangle();
    ~Rectangle();

    virtual float x() const noexcept;
    virtual float y() const noexcept;
    virtual float left() const noexcept;
    virtual float right() const noexcept;
    virtual float top() const noexcept;
    virtual float bottom() const noexcept;

    sf::RectangleShape shape;
};

#endif // RECTANGLE_HPP
