#include "ball.hpp"

const sf::Color Ball::DEFAULT_COLOUR = sf::Color(0, 0, 0);
const int Ball::DEFAULT_SPEED = 10;
const int Ball::DEFAULT_RADIUS = 7;
const int Ball::SHADOW_OPACITY = 127;
const int Ball::SLOW_DOWN_FACTOR = -2;
const int Ball::SPEED_UP_FACTOR = 2;

Ball::Ball(GameEngine *game, const sf::Vector2f position) :
    m_velocity(0, DEFAULT_SPEED),
    m_speed(DEFAULT_SPEED),
    m_radius(DEFAULT_RADIUS),
    m_isDestroyed(false)
{
    m_engine = game;

    shape.setPosition(position);
    shape.setRadius(m_radius);
    shape.setOrigin(radius() / 2, radius() / 2);
    shape.setTexture(&m_engine->resourceMan.GetTexture("ball.png"));

    shadow.setPosition(position.x + (radius() / 2), position.y + radius() / 2);
    shadow.setRadius(radius());
    shadow.setFillColor({0, 0, 0, SHADOW_OPACITY});
    shadow.setOrigin(0, 0);

    m_hitBordersSound.setBuffer(m_engine->resourceMan.GetSound("ball_hit_borders.wav"));

    m_velocity = {0, -DEFAULT_SPEED};
}

Ball::~Ball()
{

}

void Ball::Init(GameEngine *game)
{
    //shape.setTexture(m_engine->resourceMan.GetTexture("ball.png"));
}

void Ball::Draw()
{
    m_engine->getWindow().draw(shadow);
    m_engine->getWindow().draw(shape);
}

void Ball::SetPos(const sf::Vector2f newPosition) noexcept
{
    shape.setPosition(newPosition);
    shadow.setPosition(newPosition.x + GetRadius() / 2, newPosition.y + GetRadius() / 2);
}

void Ball::SetVelocity(const sf::Vector2f newVelocity) noexcept
{
    m_velocity = newVelocity;
}

int Ball::GetRadius() const noexcept
{
    return m_radius;
}

void Ball::Update(/*const float mFT, */)
{
    shape.move(m_velocity/* * mFT*/);
    shadow.move(m_velocity/* * mFT*/);

    // Rebound ball off of sides of window
    if (left() <= 20) {
        SetPos({20 + GetRadius(), y()});
        //SetVelocity({0, 0});
        m_velocity.x = -m_velocity.x;
        m_hitBordersSound.play();
    } else if (right() >= m_engine->getWindowSize().x - 20) {
        SetPos({m_engine->getWindowSize().x - 20 - GetRadius() * 2, y()});
        //SetVelocity({0, 0});
        m_velocity.x = -m_velocity.x;
        m_hitBordersSound.play();
    }

    // Rebound ball off of top of window
    if (top() <= (30 + 20)) {
        SetPos({x(), 30 + 20 + GetRadius()});
        //SetVelocity({0, 0});
        m_velocity.y = m_speed;
        m_hitBordersSound.play();
    }
}

void Ball::SlowDown() noexcept
{
    m_speed += SLOW_DOWN_FACTOR;
}

void Ball::SpeedUp() noexcept
{
    m_speed += SPEED_UP_FACTOR;
}

bool Ball::IsDestroyed() const noexcept
{
    return m_isDestroyed;
}

void Ball::Destroy() noexcept
{
    m_isDestroyed = true;
}
