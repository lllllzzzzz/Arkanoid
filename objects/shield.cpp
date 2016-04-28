#include "shield.hpp"

const int Shield::HEIGHT = 5;
const int Shield::ELEVATION = 45;
const sf::Color Shield::COLOUR = sf::Color::Green;

Shield::Shield() :
    m_isEnabled(false)
{

}

Shield::~Shield()
{

}

void Shield::Init(GameEngine *game)
{
    m_engine = game;

    shape.setPosition(0, m_engine->getWindowSize().y - ELEVATION);
    shape.setSize({m_engine->getWindowSize().x, HEIGHT});
    shape.setFillColor(COLOUR);
    shape.setOrigin(0, 0);
}

void Shield::Draw()
{
    m_engine->getWindow().draw(shape);
}
