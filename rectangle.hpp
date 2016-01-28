#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

using namespace sf;

class Rectangle
{
public:
    Rectangle();
    ~Rectangle();

    RectangleShape shape;

    float x() const noexcept;
    float y() const noexcept;
    float left() const noexcept;
    float right() const noexcept;
    float top() const noexcept;
    float bottom() const noexcept;
};

#endif // RECTANGLE_HPP
