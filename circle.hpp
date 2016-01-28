#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

using namespace sf;

class Circle
{
public:
    Circle();
    ~Circle();

    CircleShape shape;

    float x() const noexcept;
    float y() const noexcept;
    float left() const noexcept;
    float right() const noexcept;
    float top() const noexcept;
    float bottom() const noexcept;
};

#endif // CIRCLE_HPP
