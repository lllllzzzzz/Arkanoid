#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <SFML\Graphics.hpp>

class Circle
{
public:
    Circle();
    ~Circle();

    virtual float x() const noexcept;
    virtual float y() const noexcept;
    virtual float left() const noexcept;
    virtual float right() const noexcept;
    virtual float top() const noexcept;
    virtual float bottom() const noexcept;
    virtual float radius() const noexcept;

    sf::CircleShape shape;
};

#endif // CIRCLE_HPP