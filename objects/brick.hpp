#ifndef BRICK_HPP
#define BRICK_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"
#include "..\stateman\gameEngine.hpp"

class Brick : public Rectangle
{
public:
    Brick(GameEngine *game, const sf::Vector2f position, const sf::Color colour, const int type);
    ~Brick();

    void Init(GameEngine *game);

    void setPos(const sf::Vector2f newPosition);

    const bool isVisible() const noexcept;
    void setVisibility(const bool isVisible) noexcept;

    int GetType() const noexcept { return m_type; }

    int getWidth() const noexcept { return m_width; }
    int getHeight() const noexcept { return m_height; }

    bool isDestroyed() const noexcept;
    void destroy() noexcept;
    
    //Sprite sprite;

    sf::RectangleShape shadow;

private:
    sf::Vector2f velocity;

    int m_width;
    int m_height;
    int m_type;

    bool m_isDestroyed;
    bool m_isVisible;

    static const int width;
    static const int height;
    static const int shadowOpacity;
};

#endif // BRICK_HPP
