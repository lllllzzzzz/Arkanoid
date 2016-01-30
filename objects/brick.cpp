#include <ctime>

#include "Brick.hpp"

Brick::Brick(float posX, float posY)
{
    /*if (!texture.loadFromFile("data\\graphics\\brick_texture.png")) {
    }

    sprite.setTexture(texture);

    sf::IntRect subRect;
    subRect.left = 0;
    subRect.top = 0;
    subRect.width = 20;
    subRect.height = 20;

    shape.setTextureRect(subRect);

    shape.setTexture(&texture);*/

    shape.setPosition(posX, posY);
    shape.setSize({brickWidth, brickHeight});

    // Generate random colour for brick
    //std::srand(std::time(0));

    int r = std::rand() % 256;
    int g = std::rand() % 256;
    int b = std::rand() % 256;
    sf::Color color(r, g, b);
    shape.setFillColor(color);

    shape.setOrigin(brickWidth / 2.f, brickHeight / 2.f);
    isDestroyed = false;
}

Brick::~Brick()
{

}
