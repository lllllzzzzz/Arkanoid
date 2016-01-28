#ifndef BRICK_HPP
#define BRICK_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"

using namespace sf;

constexpr int brickWidth{60};
constexpr int brickHeight{20};
constexpr int countBricksX{10};
constexpr int countBricksY{4};

class Brick : public Rectangle
{
public:
    Brick(float posX, float posY);
    ~Brick();

    bool isDestroyed;

    virtual float x();
    virtual float y();
    virtual float left();
    virtual float right();
    virtual float top();
    virtual float bottom();

private:
    Vector2f velocity;
};

#endif // BRICK_HPP
