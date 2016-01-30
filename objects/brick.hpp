#ifndef BRICK_HPP
#define BRICK_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"

constexpr int brickWidth{45};
constexpr int brickHeight{15};
constexpr int countBricksX{14};
constexpr int countBricksY{8};

class Brick : public Rectangle
{
public:
    Brick(float posX, float posY);
    ~Brick();

    bool isDestroyed;

private:
    sf::Vector2f velocity;
    //Texture texture;
    //Sprite sprite;
};

#endif // BRICK_HPP
