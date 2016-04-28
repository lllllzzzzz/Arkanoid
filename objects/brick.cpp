#include <ctime>
#include <iostream>
#include "Brick.hpp"

const sf::Vector2f Brick::SIZE = {40, 20};
const int Brick::SHADOW_OPACITY = 127;

Brick::Brick(GameEngine *game, const sf::Vector2f position, const sf::Color colour, const int type) :
    m_size(SIZE),
    m_type(type),
    m_isDestroyed(false),
    m_isVisible(true)
{
    m_engine = game;

    shape.setPosition(position);
    shape.setSize(SIZE);
    shape.setFillColor(colour);
    shape.setTexture(&m_engine->resourceMan.GetTexture("brick.png"));
    shape.setOrigin(SIZE.x / 2.f, SIZE.y / 2.f);

    shadow.setPosition({position.x + (m_size.x / 2), position.y + m_size.y});
    shadow.setSize(SIZE);
    shadow.setFillColor({0, 0, 0, SHADOW_OPACITY});
    shadow.setOrigin(SIZE.x / 2.f, SIZE.y / 2.f);
}

Brick::~Brick()
{

}

void Brick::Init(GameEngine *game)
{
    //texture = m_engine->resourceMan.GetTexture("brick.png");

    //shape.setTexture(&texture);
}

void Brick::SetPos(const sf::Vector2f newPosition)
{
    shape.setPosition(newPosition);
    shadow.setPosition({newPosition.x + GetSize().x / 2, newPosition.y + GetSize().y});
}

void Brick::Draw()
{
    m_engine->getWindow().draw(shadow);
    m_engine->getWindow().draw(shape);
}

const bool Brick::IsVisible() const noexcept
{
    return m_isVisible;
}

void Brick::SetVisibility(const bool isVisible) noexcept
{
    m_isVisible = isVisible;
}

bool Brick::IsDestroyed() const noexcept
{
    return m_isDestroyed;
}

void Brick::Destroy() noexcept
{
    m_isDestroyed = true;
}
