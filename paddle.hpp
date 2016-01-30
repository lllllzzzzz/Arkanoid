#ifndef PADDLE_HPP
#define PADDLE_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"

class Paddle : public Rectangle
{
public:
    Paddle(float posX, float posY, int width, int height, sf::Color color);
    ~Paddle();

    void update();
    void setPos(const int posX, const int posY);

private:
    sf::Vector2f velocity;
    const int paddleVelocity{10.0f};
};

#endif // PADDLE_HPP
