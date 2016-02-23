#include "object.hpp"

Object::Object()
{

}

Object::~Object()
{

}

void Object::SetPosition(const sf::Vector2f newPosition) noexcept
{
    m_position = newPosition;
    //shape.setPosition(newPosition);
    //shadow.setPosition({newPosition.x + GetSize().x / 2, newPosition.y + GetSize().y});
}

sf::Vector2f Object::GetPosition() const noexcept
{
    return m_position;
}

void Object::SetSize(const sf::Vector2f size) noexcept
{

}

sf::Vector2f Object::GetSize() const noexcept
{

}

void Object::SetVisibility(const bool isVisible) noexcept
{
    m_isVisible = isVisible;
}

const bool Object::IsVisible() const noexcept
{
    return m_isVisible;
}

void Object::Destroy() noexcept
{
    m_isDestroyed = true;
}

bool Object::IsDestroyed() const noexcept
{
    return m_isDestroyed;
}
