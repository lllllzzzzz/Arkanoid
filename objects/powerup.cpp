#include "powerup.hpp"

const int Powerup::DEFAULT_SPEED = -4;
const sf::Vector2f Powerup::DEFAULT_SIZE = {40, 20};

Powerup::Powerup(GameEngine *game, const sf::Vector2f position) :
m_speed(DEFAULT_SPEED),
velocity(0, DEFAULT_SPEED),
m_width(DEFAULT_SIZE.x),
m_height(DEFAULT_SIZE.y),
m_isDestroyed(false)
{
    m_engine = game;

    velocity.x = (std::rand() % 9) - 4;

    shape.setPosition(position);
    shape.setSize(DEFAULT_SIZE);
    //shape.setFillColor(color);
    shape.setOrigin(m_width / 2.f, m_height / 2.f);
    shape.setTexture(&m_engine->resourceMan.GetTexture("powerup_extra_life.png"));

    // Set up paddle shadow shape
    shadow.setPosition(position.x + m_width / 4, position.y + (m_height / 2));
    shadow.setSize({m_width, m_height});
    shadow.setFillColor(sf::Color(0, 0, 0, 127));
    shadow.setOrigin(m_width / 2.f, m_height / 2.f);
}

Powerup::~Powerup()
{

}

void Powerup::Update()
{
    velocity.y += 0.1;

    // Move ball at velocity and angle specified by vector
    shape.move(velocity/* * mFT*/);
    shadow.move(velocity/* * mFT*/);

    if (y() >= m_engine->getWindowSize().y) {
        Destroy();
    }

    // Rebound ball off of sides of window
    if (left() <= 20) {
        //SetPos({20 + GetRadius(), y()});
        //SetVelocity({0, 0});
        velocity.x = -velocity.x;
        //m_hitBordersSound.play();
    } else if (right() >= m_engine->getWindowSize().x - 20) {
        //SetPos({m_engine->getWindowSize().x - 20 - GetRadius() * 2, y()});
        //SetVelocity({0, 0});
        velocity.x = -velocity.x;
        //m_hitBordersSound.play();
    }

    // Rebound ball off of top of window
    if (top() <= (30 + 20)) {
        //SetPos({x(), 30 + 20 + GetRadius()});
        //SetVelocity({0, 0});
        velocity.y = -velocity.y;
        //m_hitBordersSound.play();
    }
}

void Powerup::Draw()
{
    m_engine->getWindow().draw(shadow);
    m_engine->getWindow().draw(shape);
}
