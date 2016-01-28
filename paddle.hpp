#ifndef PADDLE_HPP
#define PADDLE_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"

using namespace sf;

class Paddle : public Rectangle
{
public:
    Paddle(float posX, float posY, int w, int h);
    ~Paddle();

    virtual float x();
    virtual float y();
    virtual float left();
    virtual float right();
    virtual float top();
    virtual float bottom();

    void update();
    void setPos(int posX, int posY);

private:
    const int paddleVelocity{10.0f};
    Vector2f velocity;
};

#endif // PADDLE_HPP
