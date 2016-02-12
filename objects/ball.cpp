#include "ball.hpp"

const int Ball::m_ballVelocityDefault = 10;
const int Ball::m_radiusDefault = 7;
const sf::Color Ball::m_colourDefault = sf::Color(0, 0, 0);
const int Ball::m_shadowOpacity = 127;

Ball::Ball(GameEngine *game, const float x, const float y) :
    velocity(0, m_ballVelocityDefault),
    m_ballVelocity(m_ballVelocityDefault),
    m_radius(m_radiusDefault),
    m_isDestroyed(false)
{
    shape.setPosition(x, y);
    shape.setRadius(m_radius);
    shape.setOrigin(radius() / 2, radius() / 2);
    shape.setTexture(&game->resourceMan.GetTexture("ball.png"));

    shadow.setPosition(x + (radius() / 2), y + radius() / 2);
    shadow.setRadius(radius());
    shadow.setFillColor({0, 0, 0, m_shadowOpacity});
    shadow.setOrigin(0, 0);

    hitBordersSound.setBuffer(game->resourceMan.GetSound("ball_hit_borders.wav"));
}

Ball::~Ball()
{

}

void Ball::Init(GameEngine *game)
{
    //shape.setTexture(game->resourceMan.GetTexture("ball.png"));
}

void Ball::setPos(const sf::Vector2f newPosition) noexcept
{
    shape.setPosition(newPosition);
    shadow.setPosition(newPosition.x + getRadius() / 2, newPosition.y + getRadius() / 2);
}

void Ball::setVelocity(const sf::Vector2f newVelocity) noexcept
{
    velocity = newVelocity;
}

int Ball::getRadius() const noexcept
{
    return m_radius;
}

void Ball::update(/*const float mFT, */const sf::Vector2f windowSize)
{
    // Move ball at velocity and angle specified by vector
    shape.move(velocity/* * mFT*/);
    shadow.move(velocity/* * mFT*/);

    // Rebound ball off of sides of window
    if (left() <= 20) {
        velocity.x = -velocity.x;
        hitBordersSound.play();
    } else if (right() >= windowSize.x - 20 - 10) {
        velocity.x = -velocity.x;
        hitBordersSound.play();
    }

    // Rebound ball off of top of window
    if (top() < (30 + 20)) {
        velocity.y = m_ballVelocity;
        hitBordersSound.play();
    }
}

bool Ball::isDestroyed() const noexcept
{
    return m_isDestroyed;
}

void Ball::destroy() noexcept
{
    m_isDestroyed = true;
}
