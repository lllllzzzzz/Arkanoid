#include "Paddle.hpp"

const float Paddle::DEFAULT_SPEED = 8;
const sf::Vector2f Paddle::DEFAULT_SIZE = {80, 15};
const int Paddle::ELEVATION = 70;
const int Paddle::SHADOW_OPACITY = 127;
const int Paddle::SPEED_UP_FACTOR = 2;
const int Paddle::SLOW_DOWN_FACTOR = -2;
const int Paddle::EXPAND_FACTOR = 20;
const int Paddle::SHRINK_FACTOR = -20;

Paddle::Paddle(const sf::Vector2f position, const sf::Vector2f size, sf::Color color) :
    m_size(size),
    m_speed(DEFAULT_SPEED),
    m_newWidth(0),
    m_hasLaser(false)
{
    setPos(position);
    setSize(size);
    shadow.setFillColor(sf::Color(0, 0, 0, SHADOW_OPACITY));
}

Paddle::~Paddle()
{

}

void Paddle::Init(GameEngine *game)
{
    m_engine = game;

    shape.setTexture(&game->resourceMan.GetTexture("paddle.png"));
    shadow.setTexture(&game->resourceMan.GetTexture("paddle_shadow.png"));
}

void Paddle::Draw()
{
    m_engine->getWindow().draw(shadow);
    m_engine->getWindow().draw(shape);
}

void Paddle::Reset()
{
    shape.setPosition(m_engine->getWindowSize().x / 2, m_engine->getWindowSize().y - ELEVATION);
    setSize(DEFAULT_SIZE);
    setSpeed(DEFAULT_SPEED);
}

void Paddle::update(/*const float mFT, */const sf::Vector2f windowSize)
{
    shape.move(velocity/* * mFT*/);
    shadow.move(velocity/* * mFT*/);

    // Handle player input (move paddle left or right)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && left() > 25) {
        velocity.x = -m_speed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
        && right() < (windowSize.x - 25)) {
        velocity.x = m_speed;
    } else {
        velocity.x = 0;
    }
}

void Paddle::setPos(const sf::Vector2f position) noexcept
{
    shape.setPosition(position);
    shadow.setPosition(position.x + (getSize().x / 4), position.y + (getSize().y / 2));
}

void Paddle::setSize(const sf::Vector2f size)
{
    m_size = size;

    shape.setSize(size);
    shape.setOrigin(size.x / 2.f, size.y / 2.f);

    shadow.setOrigin(size.x / 2.f, size.y / 2.f);
    shadow.setSize(size);
    shadow.setPosition(x() + (size.x / 4), y() + (size.y / 2));
}

void Paddle::setSpeed(const int speed) noexcept
{
    m_speed = speed;
}

int Paddle::getSpeed() const noexcept
{
    return m_speed;
}

void Paddle::SpeedUp() noexcept
{
    setSpeed(getSpeed() + SPEED_UP_FACTOR);
}

void Paddle::SlowDown() noexcept
{
    setSpeed(getSpeed() + SLOW_DOWN_FACTOR);
}

void Paddle::Expand() noexcept
{
    setSize({getSize().x + EXPAND_FACTOR, getSize().y});
}

void Paddle::Shrink() noexcept
{
    setSize({getSize().x + SHRINK_FACTOR, getSize().y});
}
