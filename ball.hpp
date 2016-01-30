#ifndef BALL_HPP
#define BALL_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "circle.hpp"

class Ball : public Circle
{
public:
    Ball(const float posX, const float posY, const float radius, sf::Color color);
    ~Ball();

    void update(const int windowWidth, const int windowHeight);
    void setVelocity(const int x, const int y);
    void setPos(const float posX, const float posY);

    const int ballVelocity{8.0f};
    sf::Vector2f velocity{-ballVelocity, -ballVelocity};
    const int initialPosX = 770 - 25;
    const int initialPosY = 500 - 80;

private:
    //const float ballVelocity{8.0f};
};

#endif // BALL_HPP
